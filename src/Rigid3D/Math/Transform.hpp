#ifndef RIGID3D_TRANSFORM_HPP_
#define RIGID3D_TRANSFORM_HPP_

#include <Rigid3D/Common/Settings.hpp>

namespace Rigid3D {

    class Transform {
    public:
        vec3 position;  // Position of the source space origin in destination space coordinates.
        quat pose;  // Rotation of the source space as viewed by the destination space.
        vec3 scale;

        Transform();

        Transform(const vec3 & position, const quat & pose);

        ~Transform();

        void setIdentity();

        void set(const vec3 position, const vec3 & axis, float32 angle);
    };

}

#endif /* RIGID3D_TRANSFORM_HPP_ */
