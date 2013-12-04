#ifndef BOUNDINGVOLUME_HPP_
#define BOUNDINGVOLUME_HPP_

//Forward Declarations.
namespace Rigid3D {
    struct RayCastInput;
    struct RayCastOutput;
}

namespace Rigid3D {

    /**
     * \interface BoundingVolume
     *
     * Interface for representing a bounding volume to be used during collision
     * queries.
     */
    class BoundingVolume {
    public:
        virtual ~BoundingVolume() { }

        virtual bool rayCast(const RayCastInput & input, RayCastOutput & output) const = 0;
    };

}

#endif /* BOUNDINGVOLUME_HPP_ */
