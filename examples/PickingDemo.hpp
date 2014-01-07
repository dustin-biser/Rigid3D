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

    Light * light;

    Renderable * cube;
    Renderable * sphere;
    Renderable * torus;

    ShaderProgram shader;

    void init();

    void setupShaders();
    void setupLight();
    void setupRenderables();

    void logic();
    void draw();
};

#endif /* PICKING_DEMO_HPP_ */
