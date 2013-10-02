#include <Camera.hpp>
using glm::conjugate;
using glm::normalize;
using glm::cross;
using glm::mat3;

#include <glm/gtx/transform.hpp>
using glm::translate;
using glm::transpose;

#include <glm/gtx/quaternion.hpp>
using glm::rotate;
using glm::quat_cast;

#include <glm/gtc/quaternion.hpp>
using glm::angleAxis;

#include "MathUtils.hpp"
using MathUtils::radiansToDegrees;

///////////////////////////////
// TODO (Dustin) Remove.
#include "glm/gtc/quaternion.hpp"
using glm::length;
#include <iostream>
using namespace std;
///////////////////////////////

using namespace GlUtils;

//----------------------------------------------------------------------------------------
/**
 * Default Constructor
 */
Camera::Camera()
    : recalcViewMatrix(true) {

    this->initLocalCoordinateSystem();
}

//----------------------------------------------------------------------------------------
/**
 * Constructs an orthographic projected \c Camera.
 *
 * @note If zNear or zFar are negative, corresponding z clipping planes
 * are considered behind the viewer.
 *
 * @param left - specify location of left clipping plane.
 * @param right - specify location of right clipping plane.
 * @param bottom - specify location of bottom clipping plane.
 * @param top - specify location of top clipping plane.
 * @param zNear - distance to near z clipping plane.
 * @param zFar - distance to far z clipping plane.
 */
Camera::Camera(float left, float right, float bottom, float top, float zNear, float zFar)
    : Frustum(left,  right,  bottom,  top,  zNear, zFar),
      recalcViewMatrix(true) {

    this->initLocalCoordinateSystem();
}


//----------------------------------------------------------------------------------------
/**
 * Constructs a perspective projected \c Camera.
 *
 * @note  Depth buffer precision is affected by the values specified for
 * zNear and zFar. The greater the ratio of zFar to zNear is, the less
 * effective the depth buffer will be at distinguishing between surfaces
 * that are near each other. If  r = zFar zNear roughly log2(r) bits
 * of depth buffer precision are lost. Because r approaches infinity as
 * zNear approaches 0, zNear must never be set to 0.
 *
 * @param fieldOfViewY
 * @param aspectRatio
 * @param zNear - distance to near z clipping plane (always positive).
 * @param zFar - distance to far z clipping plane (always positive).
 */
Camera::Camera(float fieldOfViewY, float aspectRatio, float zNear, float zFar)
    : Frustum(fieldOfViewY,  aspectRatio,  zNear,  zFar),
      recalcViewMatrix(true) {

    this->initLocalCoordinateSystem();
}

//----------------------------------------------------------------------------------------
/**
 * Sets the world position of the \c Camera.
 * @param x
 * @param y
 * @param z
 */
void Camera::setPosition(float x, float y, float z) {
    eyePosition.x = x;
    eyePosition.y = y;
    eyePosition.z = z;

    recalcViewMatrix = true;
}

//----------------------------------------------------------------------------------------
void Camera::initLocalCoordinateSystem() {
    l.x = -1.0f;
    l.y = 0.0f;
    l.z = 0.0f;

    u.x = 0.0f;
    u.y = 1.0f;
    u.z = 0.0f;

    f.x = 0.0f;
    f.y = 0.0f;
    f.z = -1.0f;

    eyePosition.x = 0.0f;
    eyePosition.y = 0.0f;
    eyePosition.z = 0.0f;
}

//----------------------------------------------------------------------------------------
/**
 * Sets the world position of the \c Camera.
 * @param x
 * @param y
 * @param z
 */
void Camera::setPosition(const vec3 &v) {
    setPosition(v.x, v.y, v.z);
}

//----------------------------------------------------------------------------------------
/**
 * @return the world position of the \c Camera.
 */
vec3 Camera::getPosition() const {
    return eyePosition;
}


//----------------------------------------------------------------------------------------
/**
 * @return the \c Camera's left direction vector given in world space coordinates.
 */
vec3 Camera::getLeftDirection() const {
    return l;
}

//----------------------------------------------------------------------------------------
/**
 * @return the \c Camera's up direction vector given in world space coordinates.
 */
vec3 Camera::getUpDirection() const {
    return u;
}

//----------------------------------------------------------------------------------------
/**
 * @return the \c Camera's forward direction vector given in world space coordinates.
 */
vec3 Camera::getForwardDirection() const {
    return f;
}
//----------------------------------------------------------------------------------------
/**
 * @return a 4x4 view matrix representing the \c Camera object's view transformation.
 */
mat4 Camera::getViewMatrix() const {
    if (recalcViewMatrix) {
        // Compute inverse rotation q
        quat q = orientation;
        q.x *= -1.0f;
        q.y *= -1.0f;
        q.z *= -1.0f;
        viewMatrix = mat4_cast(q);

        // Translate by inverse eyePosition.
        vec3 v = -1.0f * eyePosition;
        mat4 m = viewMatrix;
        viewMatrix[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];

        recalcViewMatrix = false;
    }

    return viewMatrix;
}

//----------------------------------------------------------------------------------------
/**
 * Rotates \c Camera about its local negative z-axis (forward direction) by \c angle radians.
 *
 * @note Rotation is counter-clockwise if \c angle > 0, and clockwise if \c angle is < 0.
 *
 * @param angle - rotation angle in radians.
 */
void Camera::roll(float angle) {
    quat q = angleAxis(angle, f);

    u = glm::rotate(q, u);
    l = glm::rotate(q, l);

    orientation = q * orientation;
    recalcViewMatrix = true;
}

//----------------------------------------------------------------------------------------
/**
 * Rotates \c Camera about its local x (left direction) axis by \c angle radians.
 *
 * @note Rotation is counter-clockwise if \c angle > 0, and clockwise if \c angle is < 0.
 *
 * @param angle - rotation angle in radians.
 */
void Camera::pitch(float angle) {
    quat q = angleAxis(angle, -1.0f * l);

    u = glm::rotate(q, u);
    f = glm::rotate(q, f);

    orientation = q * orientation;
    recalcViewMatrix = true;
}

//----------------------------------------------------------------------------------------
/**
 * Rotates \c Camera about its local y (up direction) axis by \c angle radians.
 *
 * @note Rotation is counter-clockwise if \c angle > 0, and clockwise if \c angle is < 0.
 *
 * @param angle - rotation angle in radians.
 */
void Camera::yaw(float angle) {
    quat q = angleAxis(angle, u);

    l = glm::rotate(q, l);
    f = glm::rotate(q, f);

    orientation = q * orientation;
    recalcViewMatrix = true;
}

//----------------------------------------------------------------------------------------
/**
 * Rotates \c Camera by \c angle radians about \c axis whose components are expressed using
 * the \c Camera's local coordinate system.
 *
 * @param angle
 * @param axis
 */
void Camera::rotate(float angle, const vec3 & axis) {
    vec3 n = normalize(axis);
    quat q = angleAxis(angle, n);

    l = glm::rotate(q, l);
    u = glm::rotate(q, u);
    f = glm::rotate(q, f);

    orientation = q * orientation;
    recalcViewMatrix = true;
}

//----------------------------------------------------------------------------------------
/**
 * Translates the \c Camera with respect to the world coordinate system.
 *
 * @param x
 * @param y
 * @param z
 */
void Camera::translate(float x, float y, float z) {
    eyePosition.x += x;
    eyePosition.y += y;
    eyePosition.z += z;

    recalcViewMatrix = true;
}

//----------------------------------------------------------------------------------------
/**
 * Translates the \c Camera with respect to the world coordinate system.
 *
 * @param v
 */
void Camera::translate(const vec3& v) {
    translate(v.x, v.y, v.z);
}

//----------------------------------------------------------------------------------------
/**
 * Translates the \c Camera relative to its locate coordinate system.
 *
 * @param left - translation along the \c Camera's left direction.
 * @param up - translation along the \c Camera's up direction.
 * @param forward - translation along the \c Camera's forward direction.
 */
void Camera::translateRelative(float left, float up, float forward) {
    eyePosition += left * l;
    eyePosition += up * u;
    eyePosition += forward * f;

    recalcViewMatrix = true;
}

//----------------------------------------------------------------------------------------
/**
 * Translates the \c Camera relative to its locate coordinate system.
 *
 * v.x - translation along the \c Camera's left direction.
 * v.y - translation along the \c Camera's up direction.
 * v.z - translation along the \c Camera's forward direction.
 *
 * @param v
 */
void Camera::translateRelative(const vec3& v) {
    translateRelative(v.x, v.y, v.z);
}

//----------------------------------------------------------------------------------------
void Camera::lookAt(const vec3 & center) {
    this->lookAt(center.x, center.y, center.z);
}

//----------------------------------------------------------------------------------------
void Camera::lookAt(float centerX, float centerY, float centerZ) {
    // TODO (Dustin) implement this method.
}

//----------------------------------------------------------------------------------------
void Camera::lookAt(const vec3 & eye, const vec3 & center, const vec3 & up) {
    // TODO (Dustin) What to do when eye is approx equal to center?
    eyePosition = eye;

    // Orient Camera basis vectors.
    f = normalize(center - eye);
    l = normalize(cross(up, f));
    u = cross(f, l);

    // Compute orientation from 3x3 change of basis matrix whose columns are the
    // world basis vectors given in Camera space coordinates.
    mat3 m;
    m[0] = -1.0f * l; // first column, representing new x-axis orientation
    m[1] = u;         // second column, representing new y-axis orientation
    m[2] = -1.0f * f; // third column, representing new z-axis orientation
//    m = transpose(m);

    orientation = quat_cast(m);
}
