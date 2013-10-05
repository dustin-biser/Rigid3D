/*
 * Camera.hpp
 *
 *  Created on: Sep 28, 2013
 *      Author: dustin
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
using glm::mat4_cast;

#include <Frustum.hpp>

namespace GlUtils {

    /**
     * Class for encapsulating a view frustum camera, capable of either orthographic or perspective
     * projections.  Both view and projection matrices can be retrieved from a \c Camera object at
     * any time, which reflect the current state of the \c Camera.
     *
     * Default values for a \c Camera object in world coordinates include:
     * \c position, located at world coordinate origin (0, 0, 0).
     * \c left direction, aligned with negative x-axis (-1, 0, 0).
     * \c up direction, aligned with y-axis (0, 1, 0).
     * \c forward direction, aligned with negative z-axis (0, 0, -1).
     */
    class Camera : public Frustum {
    private:
        glm::vec3 eyePosition; // Location of camera in world coordinates.
        glm::quat orientation; // Orientation of camera basis vectors specified in world coordinates.

        glm::vec3 l; // Camera's left direction vector, given in world coordinates.
        glm::vec3 u; // Camera's up direction vector, given in world coordinates.
        glm::vec3 f; // Camera's forward direction vector, given in world coordinates.

        mutable bool recalcViewMatrix;
        mutable glm::mat4 viewMatrix;

        unsigned short rotationHitCount;
        static const unsigned short rotationHitCountMax = 2000;

        void initLocalCoordinateSystem();
        void registerRotation();
        void normalizeCamera();

    public:
        Camera();
        Camera(float left, float right, float bottom, float top, float zNear, float zFar);
        Camera(float fieldOfViewY, float aspectRatio, float zNear, float zFar);

        // Setters
        void setPosition(float x, float y, float z);
        void setPosition(const glm::vec3 &v);

        // Getters
        glm::vec3 getPosition() const;
        glm::vec3 getLeftDirection() const;
        glm::vec3 getUpDirection() const;
        glm::vec3 getForwardDirection() const;
        glm::quat getOrientation() const;
        glm::mat4 getViewMatrix() const;

        // Actions
        void lookAt(const glm::vec3 & center);
        void lookAt(float centerX, float centerY, float centerZ);
        void lookAt(const glm::vec3 & eye,
                    const glm::vec3 & center,
                    const glm::vec3 & up);
        void roll(float angle);
        void pitch(float angle);
        void yaw(float angle);
        void rotate(float angle, const glm::vec3 & axis);
        void translate(float x, float y, float z);
        void translate(const glm::vec3 &v);
        void translateRelative(float left, float up, float forward);
        void translateRelative(const glm::vec3 &v);
    };
}

#endif /* CAMERA_HPP_ */
