#ifndef PICKING_DEMO_HPP_
#define PICKING_DEMO_HPP_

#include <Utils/GlfwOpenGlWindow.hpp>
#include <Rigid3D/Rigid3D.hpp>

using namespace Rigid3D;

class PickingDemo : public GlfwOpenGlWindow {
public:
    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    Renderable cube;
    Renderable sphere;
    Renderable torus;
    RenderContext renderContext;

    ShaderProgram shader;

    // Mesh and Batch Containers
    MeshConsolidator meshConsolidator;
    unordered_map<const char *, BatchInfo> batchInfoMap;

    struct LightSource {
        vec3 position;      // Light position in world space.
        vec3 rgbIntensity;  // Light intensity for each RGB component.
    };
    LightSource light;

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;

    void init();

    void setupShaders();
    void setupVertexBuffers();
    void setupRenderables();

    void logic();
    void draw();
    void cleanup();
//    void cursorPosition(double xPos, double yPos);
//    void mouseButtonInput(int button , int actions, int mods);
//    void keyInput(int key, int action, int mods);

    void computeAABB(const Renderable & r, AABB *);
};

#endif /* PICKING_DEMO_HPP_ */
