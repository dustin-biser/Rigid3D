#ifndef RAY_PICKING_DEMO_HPP_
#define RAY_PICKING_DEMO_HPP_

#include <Utils/GlfwOpenGlWindow.hpp>
#include <Rigid3D/Rigid3D.hpp>

using namespace Rigid3D;

class RayPickingDemo : public GlfwOpenGlWindow {
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
    double xWindowPos;
    double yWindowPos;

    void init();
    void logic();
    void draw();
    void setupShaders();
    void setupLight();
    void setupRenderables();
    RayCastInput createRay();

    // GLFW Call Backs
//    void keyInput(int key, int action, int mods);
    void cursorPosition(double xPos, double yPos);
    void mouseButtonInput(int button , int actions, int mods);

};

#endif /* RAY_PICKING_DEMO_HPP_ */
