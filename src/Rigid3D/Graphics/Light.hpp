/**
 * @class Light
 *
 * @author Dustin Biser
 */
#ifndef RIGID3D_LIGHT_HPP_
#define RIGID3D_LIGHT_HPP_

#include <Rigid3D/Common/Settings.hpp>

// Forward declaration.
namespace Rigid3D {
    class Scene;
}

namespace Rigid3D {

    enum class LightType : uint32 {
        Directional = 0,
        Point = 1
    };

    /**
     * Light Specification used for creating new Light objects.
     */
    struct LightSpec {
       /// Type of Light.
       /// Default is LightType::Directional.
       LightType type;

       /// Position of the Light.
       vec3 position;

       /// Direction the Light is pointing.
       vec3 direction;

       /// RGB color intensities emitted by the light.
       /// Each color component will be clamped to the float range [0,1].
       vec3 color;

       /// Used to enable or disable the Light.
       /// Default is true.
       bool isEnabled;

       LightSpec();
    };

    class Light {
    public:
       vec3 position;
       vec3 direction;
       vec3 color;
       bool isEnabled;

       LightType getLightType();

    private:
       friend class Scene;
       friend class Renderable;

       Scene & scene;
       LightType type;

       Light(Scene & scene, const LightSpec & spec);

    };

}

#endif /* RIGID3D_LIGHT_HPP_ */
