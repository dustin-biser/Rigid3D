// ModelTransform.cpp

#include "ModelTransform.hpp"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace GlUtils {

//----------------------------------------------------------------------------------------
ModelTransform::ModelTransform()
    : recalcModelMatrix(false),
      position(0.0f, 0.0f, 0.0f),
      pose(),
      scaleFactor(1.0f, 1.0f, 1.0f),
      modelMatrix() {

}

//----------------------------------------------------------------------------------------
ModelTransform::~ModelTransform() {

}

//----------------------------------------------------------------------------------------
/**
 * Set the model's world space position.
 * @param position - x,y,z world space coordinates.
 */
void ModelTransform::setPosition(const vec3& position) {
    this->position = position;
    recalcModelMatrix = true;
}

//----------------------------------------------------------------------------------------
/**
 * Set the model's world space orientation.
 * @param pose - quaternion orientation in world space.
 */
void ModelTransform::setPose(const quat& pose) {
    this->pose = pose;
    recalcModelMatrix = true;
}

//----------------------------------------------------------------------------------------
/**
 * Set the model's scaling factors along model space axes.
 * @param scale - x,y,z model space scaling factors.
 */
void ModelTransform::setScale(const vec3 & scale) {
    this->scaleFactor = scale;
    recalcModelMatrix = true;
}

//----------------------------------------------------------------------------------------
vec3 ModelTransform::getPosition() {
    return position;
}

//----------------------------------------------------------------------------------------
quat ModelTransform::getPose() {
    return pose;
}

//----------------------------------------------------------------------------------------
vec3 ModelTransform::getScale() {
    return scaleFactor;
}

//----------------------------------------------------------------------------------------
mat4 ModelTransform::getModelMatrix() {
    if (!recalcModelMatrix) {
        return modelMatrix;
    }

    // 1. Rotate
    // 2. Scale
    // 3. Translate
    mat4 rotationMatrix = glm::toMat4(pose);
    modelMatrix = glm::scale(rotationMatrix, scaleFactor);
    modelMatrix = glm::translate(modelMatrix, position);

    recalcModelMatrix = false;
    return modelMatrix;
}



} // end namespace GlUtils