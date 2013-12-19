#ifndef RIGID3D_AABB_HPP_
#define RIGID3D_AABB_HPP_

#include <Rigid3D/Common/Settings.hpp>

//Forward Declarations.
namespace Rigid3D {
    struct RayCastInput;
    struct RayCastOutput;
}

namespace Rigid3D {

    /**
     * Axis Aligned Bounding Box.
     */
    struct AABB {
        vec3 minBounds;  // minimum extensions along the x,y,z directions.
        vec3 maxBounds;  // maximum extensions along the x,y,z directions.

        bool rayCast(const RayCastInput & input, RayCastOutput * output) const;

        vec3 getCenter() const;
    };

}

#endif /* RIGID3D_AABB_HPP_ */
