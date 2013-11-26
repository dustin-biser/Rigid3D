// Renderable.cpp

#include "Renderable.hpp"
#include "ShaderProgram.hpp"
#include "MeshConsolidator.hpp"
#include "MaterialProperties.hpp"
#include "ModelTransform.hpp"

namespace GlUtils {

//---------------------------------------------------------------------------------------
Renderable::Renderable(const GLuint * vao,
                       const ShaderProgram * shaderProgram,
                       const BatchInfo * batchInfo)
    : vao(const_cast<GLuint *>(vao)),
      shaderProgram(const_cast<ShaderProgram *>(shaderProgram)),
      batchInfo(const_cast<BatchInfo *>(batchInfo)) {

}

//---------------------------------------------------------------------------------------
Renderable::Renderable()
    : vao(nullptr),
      shaderProgram(nullptr),
      batchInfo(nullptr) {

}

//---------------------------------------------------------------------------------------
Renderable::~Renderable() {

}

//---------------------------------------------------------------------------------------
void Renderable::render(const RenderContext & context) {
    if (vao == nullptr || shaderProgram == nullptr || batchInfo == nullptr) {
        return;
    }

    loadUniformData(context);

    // 1. Get and save currently bound VAO.
    // 2. Bind local VAO.
    // 3. Enable ShaderProgram
    // 4. Call glDraw*(...)
    // 5. Disable ShaderProgram
    // 6. Bind previous VAO.

    GLint prev_vao;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev_vao);
    glBindVertexArray(*vao);

    shaderProgram->enable();
        glDrawArrays(GL_TRIANGLES, batchInfo->startIndex, batchInfo->numIndices);
    shaderProgram->disable();

    glBindVertexArray(prev_vao);

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void Renderable::setShaderProgram(const ShaderProgram * shaderProgram) {
    this->shaderProgram = const_cast<ShaderProgram *>(shaderProgram);
}

//---------------------------------------------------------------------------------------
void Renderable::setPosition(const vec3 & position) {
    modelTransform.setPosition(position);
}

//---------------------------------------------------------------------------------------
void Renderable::setPose(const quat & pose) {
    modelTransform.setPose(pose);
}

//---------------------------------------------------------------------------------------
void Renderable::setScale(const vec3 & scale) {
    modelTransform.setScale(scale);
}

//---------------------------------------------------------------------------------------
void Renderable::setEmissionLevels(const vec3 & emissionLevels) {
    material.emission = emissionLevels;
}

//---------------------------------------------------------------------------------------
void Renderable::setAmbientLevels(const vec3 & ambientLevels) {
    material.Ka = ambientLevels;
}

//---------------------------------------------------------------------------------------
void Renderable::setDiffuseLevels(const vec3 & diffuseLevels) {
    material.Kd = diffuseLevels;
}

//---------------------------------------------------------------------------------------
void Renderable::setSpecularIntensity(float specular) {
    material.Ks = specular;
}

//---------------------------------------------------------------------------------------
void Renderable::setShininessFactor(float shininessfactor) {
    material.shininessFactor = shininessfactor;
}

//---------------------------------------------------------------------------------------
void Renderable::loadUniformData(const RenderContext & context) {
    mat4 modelView = context.viewMatrix * modelTransform.getModelMatrix();
    shaderProgram->setUniform("ModelViewMatrix", modelView);
    shaderProgram->setUniform("ProjectionMatrix", context.projectionMatrix);

    //XXX Dustin - Find a less expensive way to compute Normal Matrix, than taking inverse.

    // Compute 3x3 normal matrix.
    // Inverse scaling factors.
//    vec3 scale = modelTransform.getScale();
//    modelView[0][0] = 1.0f / scale.x;
//    modelView[1][1] = 1.0f / scale.y;
//    modelView[2][2] = 1.0f / scale.z;
//    shaderProgram->setUniform("NormalMatrix", mat3(modelView));
    shaderProgram->setUniform("NormalMatrix", glm::transpose(glm::inverse(mat3(modelView))));

    shaderProgram->setUniform("material.emission", material.emission);
    shaderProgram->setUniform("material.Ka", material.Ka);
    shaderProgram->setUniform("material.Kd", material.Kd);
    shaderProgram->setUniform("material.Ks", material.Ks);
    shaderProgram->setUniform("material.shininessFactor", material.shininessFactor);
}


} // end namespace GlUtils

