#include "RenderableXXX.hpp"

#include <Rigid3D/Graphics/ShaderProgram.hpp>
#include <Rigid3D/Graphics/MeshConsolidator.hpp>
#include <Rigid3D/Graphics/MaterialProperties.hpp>
#include <Rigid3D/Graphics/ModelTransform.hpp>
#include <Rigid3D/Graphics/GlErrorCheck.hpp>

namespace Rigid3D {

//---------------------------------------------------------------------------------------
RenderableXXX::RenderableXXX(const GLuint * vao,
                       const ShaderProgram * shaderProgram,
                       const BatchInfo * batchInfo)
    : vao(const_cast<GLuint *>(vao)),
      shaderProgram(const_cast<ShaderProgram *>(shaderProgram)),
      batchInfo(const_cast<BatchInfo *>(batchInfo)) {

}

//---------------------------------------------------------------------------------------
RenderableXXX::RenderableXXX()
    : vao(nullptr),
      shaderProgram(nullptr),
      batchInfo(nullptr) {

}

//---------------------------------------------------------------------------------------
// Initialize reasonable default values.
void RenderableXXX::init() {
    material.emission = vec3(0.0f);
    material.Ka = vec3(1.0f);
    material.Kd = vec3(1.0f);
    material.Ks = 1.0f;
    material.shininessFactor = 1.0f;
}

//---------------------------------------------------------------------------------------
RenderableXXX::~RenderableXXX() {

}

//---------------------------------------------------------------------------------------
void RenderableXXX::render(const RenderContext & context) {
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

    Rigid3D::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
/**
 * Uses 'shaderProgram' for when Renderable::render() is called.
 * @note Only only ShaderProgram can be set at a time.
 * @note Uniform variables for the ShaderProgram will be modified.
 *
 * @param shaderProgram
 */
void RenderableXXX::setShaderProgram(ShaderProgram & shaderProgram) {
    this->shaderProgram = const_cast<ShaderProgram *>(&shaderProgram);
}

//---------------------------------------------------------------------------------------
void RenderableXXX::setPosition(const vec3 & position) {
    modelTransform.setPosition(position);
}

//---------------------------------------------------------------------------------------
void RenderableXXX::setPose(const quat & pose) {
    modelTransform.setPose(pose);
}

//---------------------------------------------------------------------------------------
void RenderableXXX::setScale(const vec3 & scale) {
    modelTransform.setScale(scale);
}

//---------------------------------------------------------------------------------------
void RenderableXXX::setEmissionLevels(const vec3 & emissionLevels) {
    material.emission = emissionLevels;
}

//---------------------------------------------------------------------------------------
void RenderableXXX::setAmbientLevels(const vec3 & ambientLevels) {
    material.Ka = ambientLevels;
}

//---------------------------------------------------------------------------------------
void RenderableXXX::setDiffuseLevels(const vec3 & diffuseLevels) {
    material.Kd = diffuseLevels;
}

//---------------------------------------------------------------------------------------
void RenderableXXX::setSpecularIntensity(float specular) {
    material.Ks = specular;
}

//---------------------------------------------------------------------------------------
void RenderableXXX::setShininessFactor(float shininessfactor) {
    material.shininessFactor = shininessfactor;
}

//---------------------------------------------------------------------------------------
void RenderableXXX::loadUniformData(const RenderContext & context) {
    mat4 modelView = context.viewMatrix * modelTransform.getModelMatrix();

    shaderProgram->setUniform("ModelViewMatrix", modelView);
    shaderProgram->setUniform("ProjectionMatrix", context.projectionMatrix);
    shaderProgram->setUniform("NormalMatrix", glm::transpose(glm::inverse(mat3(modelView))));

    shaderProgram->setUniform("material.emission", material.emission);
    shaderProgram->setUniform("material.Ka", material.Ka);
    shaderProgram->setUniform("material.Kd", material.Kd);
    shaderProgram->setUniform("material.Ks", material.Ks);
    shaderProgram->setUniform("material.shininessFactor", material.shininessFactor);
}


} // end namespace GlUtils

