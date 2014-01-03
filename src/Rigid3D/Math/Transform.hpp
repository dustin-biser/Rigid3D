#ifndef RIGID3D_TRANSFORM_HPP_
#define RIGID3D_TRANSFORM_HPP_

#include <Rigid3D/Common/Settings.hpp>

namespace Rigid3D {

    class Transform {
    public:
        vec3 position;
        quat pose;
        vec3 scale;

        Transform();

        Transform(const vec3 & position, const quat & pose);

        Transform(const vec3 & position, const quat & pose, const vec3 & scale);

        void setIdentity();

        void set(const vec3 position, const vec3 & axis, float32 angle);
    };

}

#endif /* RIGID3D_TRANSFORM_HPP_ */
