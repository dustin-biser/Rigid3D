#ifndef PICKING_DEMO_HPP_
#define PICKING_DEMO_HPP_

#include <Utils/GlfwOpenGlWindow.hpp>
#include <Rigid3D/Rigid3D.hpp>

using namespace Rigid3D;

class PickingDemo : public GlfwOpenGlWindow {
public:
    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    Scene scene;

    Renderable * cube;
    Renderable * sphere;
    Renderable * torus;

    ShaderProgram shader;

    struct LightSource {
        vec3 position;      // Light position in world space.
        vec3 rgbIntensity;  // Light intensity for each RGB component.
    };
    LightSource light;

    void init();

    void setupShaders();
    void setupRenderables();

    void logic();
    void draw();
};

#endif /* PICKING_DEMO_HPP_ */
