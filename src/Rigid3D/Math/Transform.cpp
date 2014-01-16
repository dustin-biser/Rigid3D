#include "Transform.hpp"

namespace Rigid3D {

//----------------------------------------------------------------------------------------
/**
 *  Default Constructor.
 */
Transform::Transform()
    : position(),
      pose(),
      scale(1.0f, 1.0f, 1.0f) {

}

//----------------------------------------------------------------------------------------
/**
 * Constructs a Transform from a position and pose, using identity scaling.
 *
 * @param position
 * @param pose
 */
Transform::Transform(const vec3 & position, const quat & pose)
    : position(position),
      pose(pose),
      scale(1.0f, 1.0f, 1.0f) {

}

//----------------------------------------------------------------------------------------
/**
 * Construct a Transform from a position, pose, and axis scaling.
 * @param position
 * @param pose
 * @param scale
 */
Transform::Transform(const vec3 & position, const quat & pose, const vec3 & scale)
    : position(position),
      pose(pose),
      scale(scale) {

}

//----------------------------------------------------------------------------------------
void Transform::setIdentity() {
    position.x = 0;
    position.y = 0;
    position.z = 0;

    pose.w = 1;
    pose.x = 0;
    pose.y = 0;
    pose.z = 0;

    scale.x = 1;
    scale.y = 1;
    scale.z = 1;
}

//----------------------------------------------------------------------------------------
/**
 * Sets the Transform position and pose using angle-axis format.
 * @param position
 * @param axis - rotation axis.
 * @param angle - rotation angle in radians.
 */
void Transform::set(const vec3 position, const vec3 & axis, float32 angle) {
    this->position = position;
    this->pose = glm::angleAxis(angle, axis);
}

} // end namespace Rigid3D.
