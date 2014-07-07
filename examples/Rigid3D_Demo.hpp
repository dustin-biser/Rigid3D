/**
 * @brief Rigid3D Demo
 *
 * @author Dustin Biser
 */

#pragma once

#include "Utils/GlfwOpenGlWindow.hpp"

#include <Rigid3D/Rigid3D.hpp>

#include <memory>
#include <unordered_map>
using std::unordered_map;
using std::shared_ptr;

using namespace Rigid3D;

class Rigid3D_Demo : public GlfwOpenGlWindow {

public:
    ~Rigid3D_Demo() { }

    static std::shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    Rigid3D_Demo(); // Singleton. Prevent direct construction.

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
