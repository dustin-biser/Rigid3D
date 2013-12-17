#ifndef BOUNDINGVOLUMEFACTORY_HPP_
#define BOUNDINGVOLUMEFACTORY_HPP_

// Forward Declarations.
namespace Rigid3D{
    class BoundingVolume;
    class Geometry;
}

namespace Rigid3D {

    /**
     * \interface BoundingVolumeFactory
     *
     * @note Users should never call free or delete on pointers to
     * BoundingVolume types. Memory associated with factory created
     * BoundingVolumes will automatically be freed when the factory's destructor
     * is called.
     */
    class BoundingVolumeFactory {
    public:
        virtual ~BoundingVolumeFactory() { }

        virtual BoundingVolume * create(const Geometry & geometry) = 0;

    };

}

#endif /* BOUNDINGVOLUMEFACTORY_HPP_ */
