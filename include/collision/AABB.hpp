#ifndef AABB_HPP_
#define AABB_HPP_

#include "glm/glm.hpp"
using glm::vec3;

//Forward Defines
namespace Rigid3D {
    struct RayCastInput;
    struct RayCastOutput;
}

namespace Rigid3D {

    /**
     * Axis Aligned Bounding Box
     */
    class AABB {
    public:
        vec3 min;  // minimum extensions along the x,y,z directions.
        vec3 max;  // maximum extensions along the x,y,z directions.

        bool rayCast(const RayCastInput & input, RayCastOutput & output);
    };

}

#endif /* AABB_HPP_ */
