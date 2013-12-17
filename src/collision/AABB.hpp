#ifndef AABB_HPP_
#define AABB_HPP_

#include "BoundingVolume.hpp"

#include "glm/glm.hpp"
using glm::vec3;

//Forward Declarations.
namespace Rigid3D {
    struct RayCastInput;
    struct RayCastOutput;
}

namespace Rigid3D {

    /**
     * Axis Aligned Bounding Box.
     *
     * @note For slightly better performance and precision during collision
     * detection, it is recommended to use local-coordinates for AABB::min and
     * AABB::max of the bounded shape, rather than world-coordinates.
     */
    class AABB : public BoundingVolume {
    public:
        vec3 min;  // minimum extensions along the x,y,z directions.
        vec3 max;  // maximum extensions along the x,y,z directions.

        ~AABB();

        bool rayCast(const RayCastInput & input, RayCastOutput & output) const;
    };

}

#endif /* AABB_HPP_ */
