#include "Material.hpp"

namespace Rigid3D {

Material::Material()
    : emission(),
      Ka(1.0f),
      Kd(1.0f),
      Ks(1.0f),
      shininess(1.0f) {

}

} // end namespace Rigid3D
