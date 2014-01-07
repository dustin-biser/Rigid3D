#include "Renderable.hpp"

#include <Rigid3D/Common/Rigid3DException.hpp>
#include <Rigid3D/Graphics/Camera.hpp>
#include <Rigid3D/Graphics/GlErrorCheck.hpp>
#include <Rigid3D/Graphics/Light.hpp>
#include <Rigid3D/Graphics/Scene.hpp>
#include <Rigid3D/Graphics/ShaderProgram.hpp>

#include <cstdio>
#include <iostream>
#include <sstream>

namespace Rigid3D {

using std::endl;
using std::stringstream;
using std::vector;


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
Renderable::Renderable(Scene & scene, const RenderableSpec & spec)
    : scene(scene),
      shader(nullptr) {

    checkShaderIsNotNull(spec.shader);
    setShader(*(spec.shader));

    this->meshName = spec.meshName;
    this->material = spec.material;
    this->modelTransform.set(spec.transform);
    this->cull = spec.cull;

}

//----------------------------------------------------------------------------------------
void Renderable::loadShaderUniforms(const Camera & camera) {
    mat4 modelView = camera.getViewMatrix() * modelTransform.getModelMatrix();

    shader->setUniform("ModelViewMatrix", modelView);
    shader->setUniform("ProjectionMatrix", camera.getProjectionMatrix());
    shader->setUniform("NormalMatrix", glm::transpose(glm::inverse(mat3(modelView))));

    //-- Upload material properties
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


//----------------------------------------------------------------------------------------
void Renderable::setShader(const ShaderProgram & shader) {
    if (this->shader != nullptr) {
        // Remove this Renderable from its current shader's Renderable Set.
        scene.shaderRenderableMap[this->shader].erase(this);
    }

    // Now add this Renderable to the new shader's Renderable Set.
    ShaderProgram * shaderPtr = const_cast<ShaderProgram *>(&shader);
    scene.shaderRenderableMap[shaderPtr].insert(this);
    this->shader = const_cast<ShaderProgram *>(&shader);
}

//----------------------------------------------------------------------------------------
ShaderProgram & Renderable::getShader() const {
    return *shader;
}

//----------------------------------------------------------------------------------------
void Renderable::checkShaderIsNotNull(const ShaderProgram * shader) const {
    if ( (shader == 0) || (shader == nullptr) ) {
        stringstream errorMessage;
        errorMessage << "Exception thrown from class Rigid3D::Renderable." << endl;
        errorMessage << "ShaderProgram cannot be null." << endl;

        throw Rigid3DException(errorMessage.str());
    }
}

} // end namespace Rigid3D
