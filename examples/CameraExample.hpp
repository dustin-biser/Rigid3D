/**
 * @brief CameraExample.hpp
 *
 * @author Dustin Biser
 */

#ifndef CAMERAEXAMPLE_HPP_
#define CAMERAEXAMPLE_HPP_

#include "GlfwOpenGlWindow.hpp"
#include "Rigid3D.hpp"

#include <unordered_map>
using std::unordered_map;

using namespace Rigid3D;

class CameraExample : public GlfwOpenGlWindow {

public:
    ~CameraExample() { }

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    CameraExample(); // Singleton. Prevent direct construction.

    // Mesh and Batch Containers
    MeshConsolidator meshConsolidator;
    unordered_map<const char *, BatchInfo> batchInfoMap;

    // Renderable Objects
    Renderable grid;
    Renderable bunny;
    Renderable tyrannosaurus;
    Renderable sphere;
    Renderable light;

    RenderContext renderContext;

    ShaderProgram shaderProgram;

    struct LightSource {
        vec3 position;      // Light position in world space.
        vec3 rgbIntensity;  // Light intensity for each RGB component.
    };
    LightSource lightSource;

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;

    virtual void init();
    virtual void logic();
    virtual void draw();
    virtual void keyInput(int key, int action, int mods) { }
    virtual void cleanup();

    void setupShaders();
    void setupRenderables();
    void setupGLBuffers();
};

#endif /* CAMERAEXAMPLE_HPP_ */
