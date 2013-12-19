#ifndef RIGID3D_RAYCASTOUTPUT_HPP_
#define RIGID3D_RAYCASTOUTPUT_HPP_

namespace Rigid3D {

    /**
     * Ray casting output data.  The ray hits at 'hitPoint = p1 + length * (p2 -
     * p1) / ||p2 - p1||' where 'p1' and 'p2' come from RayCastInput.
     */
    struct RayCastOutput {
        vec3 hitPoint;
        vec3 normal; // Face/Edge normal struck by ray.
        float length; // Length from ray starting point to intercepted face/edge.
    };

}

#endif /* RIGID3D_RAYCASTOUTPUT_HPP_ */
