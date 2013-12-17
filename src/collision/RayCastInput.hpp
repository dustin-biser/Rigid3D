#ifndef RAYCASTINPUT_HPP_
#define RAYCASTINPUT_HPP_

#include "glm/glm.hpp"
using glm::vec3;

namespace Rigid3D {

    /**
     * Ray casting input data.  Represents a ray starting at point 'p0' and
     * extending to point 'p0 + maxLength * (p1 - p0) / ||p1 - p0||'.
     */
    struct RayCastInput {
        vec3 p0;
        vec3 p1;
        float maxLength;
    };

}

#endif /* RAYCASTINPUT_HPP_ */
