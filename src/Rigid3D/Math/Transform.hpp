#ifndef RIGID3D_TRANSFORM_HPP_
#define RIGID3D_TRANSFORM_HPP_

#include <Rigid3D/Common/Settings.hpp>

namespace Rigid3D {

    class Transform {
    public:
        vec3 position;
        quat pose;

        Transform();

        Transform(const vec3 & position, const quat & pose);

        ~Transform();

        void setIdentity();
    };

}

#endif /* RIGID3D_TRANSFORM_HPP_ */
