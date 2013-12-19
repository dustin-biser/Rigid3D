#ifndef RIGID3D_POLYHEDRONSHAPE_HPP_
#define RIGID3D_POLYHEDRONSHAPE_HPP_

#include <Rigid3D/Common/Settings.hpp>

// Forward Declarations
namespace Rigid3D {
    class AABB;
    class Shape;
    class RayCastInput;
    class RayCastOutput;
    class Transform;
}

namespace Rigid3D {

    class PolyhedronShape : public Shape {
    public:
        /// Overrides Shape::computeAABB
        void computeAABB(AABB * aabb, const Transform & t) const;

        /// Overrides Shape::rayCast
        void rayCast(const RayCastInput &, RayCastOutput *, const Transform &) const;

    private:
        vec3 vertices[];
        int32 num_vertices;
    };

}


#endif /* RIGID3D_POLYHEDRONSHAPE_HPP_ */
