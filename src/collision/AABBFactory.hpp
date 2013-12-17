#ifndef AABBFACTORY_HPP_
#define AABBFACTORY_HPP_

// Forward Declaration.
namespace Rigid3D {
    class BoundingVolumeFactory;
    class Geometry;
}

namespace Rigid3D {

    class AABBFactory : public BoundingVolumeFactory {
    public:
        ~AABBFactory();

        BoundingVolume * create(const Geometry & geometry) = 0;
    };
}



#endif /* AABBFACTORY_HPP_ */
