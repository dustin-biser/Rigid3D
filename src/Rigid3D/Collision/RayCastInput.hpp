#ifndef RIGID3D_RAYCASTINPUT_HPP_
#define RIGID3D_RAYCASTINPUT_HPP_

#include <Rigid3D/Common/Settings.hpp>

namespace Rigid3D {

    /**
     * Ray casting input data.  Represents a ray starting at point 'p1' and
     * extending to point 'p1 + maxLength * (p2 - p1) / ||p2 - p1||'.
     */
    struct RayCastInput {
        vec3 p1;
        vec3 p2;
        float maxLength; // Maximum distance to extend ray.
    };

}

#endif /* RIGID3D_RAYCASTINPUT_HPP_ */
