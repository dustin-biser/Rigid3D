#include "Renderable.hpp"

#include <Rigid3D/Graphics/Camera.hpp>
#include <Rigid3D/Graphics/GlErrorCheck.hpp>
#include <Rigid3D/Graphics/Light.hpp>
#include <Rigid3D/Graphics/Scene.hpp>
#include <Rigid3D/Graphics/ShaderProgram.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace Rigid3D {

//----------------------------------------------------------------------------------------
ModelTransform::ModelTransform()
    : transform(),
      modelMatrix(),
      recalcModelMatrix(false) {

}

//----------------------------------------------------------------------------------------
void ModelTransform::set(const Transform & transform) {
    this->transform = transform;
    recalcModelMatrix = true;
}

//----------------------------------------------------------------------------------------
const Transform & ModelTransform::getTransform() const {
    return transform;
}

//----------------------------------------------------------------------------------------
mat4 ModelTransform::getModelMatrix() const {
    if (!recalcModelMatrix) {
        return modelMatrix;
    }

    // 1. Scale
    // 2. Rotate
    // 3. Translate

    // Scale
    mat3 matrix;
    matrix = mat3();
    matrix[0][0] = transform.scale[0];
    matrix[1][1] = transform.scale[1];
    matrix[2][2] = transform.scale[2];

    // Rotate
    matrix = glm::toMat3(transform.pose) * matrix;
    modelMatrix = mat4(matrix);

    // Translate
    modelMatrix[3][0] = transform.position.x;
    modelMatrix[3][1] = transform.position.y;
    modelMatrix[3][2] = transform.position.z;

    recalcModelMatrix = false;

    return modelMatrix;
}

//----------------------------------------------------------------------------------------
RenderableSpec::RenderableSpec()
    : meshName(),
      shader(nullptr),
      material(),
      transform(),
      cull(false) {

}

//----------------------------------------------------------------------------------------
Renderable::Renderable(const Scene & scene, const RenderableSpec & spec)
    : scene(const_cast<Scene &>(scene)) {

    this->meshName = spec.meshName;
    this->shader = spec.shader;
    this->material = spec.material;
    this->modelTransform.set(spec.transform);
    this->cull = spec.cull;
}

//----------------------------------------------------------------------------------------
/**
 * Render the Renderable from the viewpoint of the Camera parameter.
 *
 * @param camera
 */
void Renderable::render(const Camera & camera) {
    if (cull) return;

    BatchInfo batchInfo = scene.meshBatchMap[meshName];

    loadShaderUniforms(camera);

    if(scene.meshDataMap[meshName].hasTextureCoords) {
        glBindVertexArray(scene.vao_textured);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.indexBuffer_textured);
    } else {
        glBindVertexArray(scene.vao_nonTextured);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.indexBuffer_nonTextured);
    }

    shader->enable();
    glDrawElementsBaseVertex(GL_TRIANGLES, batchInfo.numIndices, GL_UNSIGNED_SHORT,
            reinterpret_cast<void *>(batchInfo.startIndex * sizeof(uint16)),
            batchInfo.baseVertex);
    shader->disable();

    #ifdef DEBUG
        checkGlErrors(__FILE__, __LINE__);
    #endif
}

//----------------------------------------------------------------------------------------
void Renderable::loadShaderUniforms(const Camera & camera) {
    mat4 modelView = camera.getViewMatrix() * modelTransform.getModelMatrix();

    GLint activeProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(shader->getProgramObject());

    // Upload ModelViewMatrix uniform data.
    glUniformMatrix4fv(scene.MODEL_VIEW_MATRIX_UNIFORM_LOCATION, 1, GL_FALSE,
        glm::value_ptr(modelView));

    // Upload ProjrectionMatrix uniform data.
    glUniformMatrix4fv(scene.PROJECTION_MATRIX_UNIFORM_LOCATION, 1, GL_FALSE,
        glm::value_ptr(camera.getProjectionMatrix()));

    mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));

    // Upload NormalMatrix uniform data.
    glUniformMatrix4fv(scene.NORMAL_MATRIX_UNIFORM_LOCATION, 1, GL_FALSE,
        glm::value_ptr(normalMatrix));

    //-- Upload material
    glUniform3fv(scene.MATERIAL_UNIFORM_EMISSION_OFFSET, 1, glm::value_ptr(material.emission));
    glUniform3fv(scene.MATERIAL_UNIFORM_KA_OFFSET, 1, glm::value_ptr(material.Ka));
    glUniform3fv(scene.MATERIAL_UNIFORM_KD_OFFSET, 1, glm::value_ptr(material.Kd));
    glUniform1f(scene.MATERIAL_UNIFORM_KS_OFFSET, material.Ks);
    glUniform1f(scene.MATERIAL_UNIFORM_SHININESS_OFFSET, material.shininess);

    uint32 numActiveLights = scene.lights.size();
    vec3 lightPos;  // Light position in eye coords.
    vec3 lightDir;  // Light direction in eye coords.
    vec4 tmp;
    GLint numFields = scene.NUM_LIGHT_UNIFORM_FIELDS;

    // Upload all active light uniform data.
    for (uint32 i = 0; i < numActiveLights; i++) {
        lightPos = scene.lights[i]->position;
        tmp.x = lightPos.x;
        tmp.y = lightPos.y;
        tmp.z = lightPos.z;
        tmp = (modelView * tmp);

        lightPos.x = tmp.x;
        lightPos.y = tmp.y;
        lightPos.z = tmp.z;

        // Upload the light's type uniform
        glUniform1ui(scene.LIGHT_UNIFORM_TYPE_OFFSET + (numFields * i),
                (uint32)(scene.lights[i]->type));

        // Upload light position uniform in eye coords.
        glUniform3fv(scene.LIGHT_UNIFORM_POSITION_OFFSET + (numFields * i), 1,
                glm::value_ptr(lightPos));

        if (scene.lights[i]->type == LightType::Directional) {
            // Upload light direction uniform in eye coords.
            lightDir = normalMatrix * scene.lights[i]->direction;
            glUniform3fv(scene.LIGHT_UNIFORM_DIRECTION_OFFSET + (numFields * i), 1,
                    glm::value_ptr(lightDir));
        }

        // Upload light color uniform.
        glUniform3fv(scene.LIGHT_UNIFORM_COLOR_OFFSET + (numFields * i), 1,
                glm::value_ptr(scene.lights[i]->color));

        // Enable the light for use in the fragment shader stage.
        glUniform1ui(scene.LIGHT_UNIFORM_ENABLED_OFFSET + (numFields * i), 1);

    }

    // Restore the previously bound shader program.
    glUseProgram(activeProgram);
}

//----------------------------------------------------------------------------------------
void Renderable::setTransform(const Transform & transform) {
    modelTransform.set(transform);
}

//----------------------------------------------------------------------------------------
const Transform & Renderable::getTransform() const {
    return modelTransform.transform;
}

} // end namespace Rigid3D
