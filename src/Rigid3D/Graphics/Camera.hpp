#ifndef RIGID3D_CAMERA_HPP_
#define RIGID3D_CAMERA_HPP_

#include <Rigid3D/Common/Settings.hpp>
#include <Rigid3D/Graphics/Frustum.hpp>

#include <glm/gtc/quaternion.hpp>

namespace Rigid3D {

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
    public:
        Camera();
        Camera(float left, float right, float bottom, float top, float zNear, float zFar);
        Camera(float fieldOfViewY, float aspectRatio, float zNear, float zFar);

        // Setters
        void setPosition(float x, float y, float z);
        void setPosition(const vec3 &v);

        // Getters
        vec3 getPosition() const;
        vec3 getLeftDirection() const;
        vec3 getUpDirection() const;
        vec3 getForwardDirection() const;
        quat getOrientation() const;
        mat4 getViewMatrix() const;

        // Actions
        void lookAt(const vec3 & center);
        void lookAt(float centerX, float centerY, float centerZ);
        void lookAt(const vec3 & eye,
                    const vec3 & center,
                    const vec3 & up);
        void roll(float angle);
        void pitch(float angle);
        void yaw(float angle);
        void rotate(float angle, const vec3 & axis);
        void translate(float x, float y, float z);
        void translate(const vec3 &v);
        void translateLocal(float left, float up, float forward);
        void translateLocal(const vec3 &v);

    private:
        vec3 eyePosition; // Location of camera in world coordinates.
        quat orientation; // Orientation of camera basis vectors specified in world coordinates.

        vec3 l; // Camera's left direction vector, given in world coordinates.
        vec3 u; // Camera's up direction vector, given in world coordinates.
        vec3 f; // Camera's forward direction vector, given in world coordinates.

        mutable bool recalcViewMatrix;
        mutable mat4 viewMatrix;

        unsigned short rotationHitCount;

        // Normalize Camera vectors after rotating this many times.
        static const unsigned short rotationHitCountMax = 1000;

        void initLocalCoordinateSystem();
        void registerRotation();
        void normalizeCamera();

    };
}

#endif /* RIGID3D_CAMERA_HPP_ */
