#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include "glm/glm.hpp"
using glm::vec3;

#include "glm/gtc/quaternion.hpp"
using glm::quat;

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

#endif /* TRANSFORM_HPP_ */
