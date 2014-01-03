#include "Renderable.hpp"

#include <Rigid3D/Graphics/Scene.hpp>
#include <Rigid3D/Graphics/ShaderProgram.hpp>
#include <Rigid3D/Graphics/Camera.hpp>
#include <Rigid3D/Graphics/GlErrorCheck.hpp>

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
    : scene(const_cast<Scene * const>(&scene)) {

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

    BatchInfo batchInfo = scene->meshBatchMap[meshName];

    loadShaderUniforms(camera);

    if(scene->meshDataMap[meshName].hasTextureCoords) {
        glBindVertexArray(scene->vao_textured);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene->indexBuffer_textured);
    } else {
        glBindVertexArray(scene->vao_nonTextured);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene->indexBuffer_nonTextured);
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

    shader->setUniform("ModelViewMatrix", modelView);
    shader->setUniform("ProjectionMatrix", camera.getProjectionMatrix());
    shader->setUniform("NormalMatrix", glm::transpose(glm::inverse(mat3(modelView))));

    shader->setUniform("material.emission", material.emission);
    shader->setUniform("material.Ka", material.Ka);
    shader->setUniform("material.Kd", material.Kd);
    shader->setUniform("material.Ks", material.Ks);
    shader->setUniform("material.shininess", material.shininess);
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
