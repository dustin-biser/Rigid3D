#include "Renderable.hpp"

namespace Rigid3D {

Renderable::Renderable(const Scene & scene, const string & meshName, const BatchInfo & batchInfo)
    : scene(&scene),
      meshName(meshName),
      batchInfo(batchInfo) {

}

//----------------------------------------------------------------------------------------

} // end namespace Rigid3D
