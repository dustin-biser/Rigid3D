#include "Light.hpp"

#include <Rigid3D/Graphics/Scene.hpp>

namespace Rigid3D {

//----------------------------------------------------------------------------------------
LightSpec::LightSpec()
    : type(LightType::Directional),
      position(),
      direction(),
      color(),
      isEnabled(true) {

}

//----------------------------------------------------------------------------------------
Light::Light(Scene & scene, const LightSpec & spec)
    : position(spec.position),
      direction(spec.direction),
      color(spec.color),
      isEnabled(spec.isEnabled),
      scene(scene),
      type(spec.type) {

}

//----------------------------------------------------------------------------------------
LightType Light::getLightType() {
   return type;
}

} // end namespace Rigid3D
