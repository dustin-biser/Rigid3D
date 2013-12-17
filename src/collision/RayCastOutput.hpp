#ifndef RAYCASTOUTPUT_HPP_
#define RAYCASTOUTPUT_HPP_

namespace Rigid3D {

    /**
     * Ray casting output data.  The ray hits at 'hitPoint = p0 + length * (p1 -
     * p0) / ||p1 - p0||' where 'p0' and 'p1' come from RayCastInput.
     */
    struct RayCastOutput {
        vec3 hitPoint;
        float length;
    };

}

#endif /* RAYCASTOUTPUT_HPP_ */
