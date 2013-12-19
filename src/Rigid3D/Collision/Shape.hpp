#ifndef RIGID3D_SHAPE_HPP_
#define RIGID3D_SHAPE_HPP_

// Forward Declarations
namespace Rigid3D {
    class AABB;
    class RayCastInput;
    class RayCastOutput;
    class Transform;
}

namespace Rigid3D {

    /***
     * \interface Shape
     */
    class Shape {
    public:
        virtual ~Shape() { }

        virtual void computeAABB(AABB * aabb, const Transform &) const = 0;

        virtual void rayCast(const RayCastInput &, RayCastOutput *, const Transform &) const = 0;

    };

}

#endif /* RIGID3D_SHAPE_HPP_ */
