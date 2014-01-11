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

    Light * whiteLight;

    RenderableSpec cubeSpec;
    Renderable * cube;

    RenderableSpec sphereSpec;
    Renderable * sphere;

    RenderableSpec torusSpec;
    Renderable * torus;

    RenderableSpec lightSpec;
    Renderable * lightMesh;

    ShaderProgram shader;

    bool key_f1_down;

    void init();

    void setupShaders();
    void setupLight();
    void setupRenderables();

    void logic();
    void draw();
    void keyInput(int key, int action, int mods);
};

#endif /* PICKING_DEMO_HPP_ */
