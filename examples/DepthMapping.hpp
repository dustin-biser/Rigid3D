#ifndef DEPTH_MAPPING_HPP
#define DEPTH_MAPPING_HPP

#include "GlfwOpenGlWindow.hpp"
#include "Rigid3D.hpp"

#include <glm/glm.hpp>
using glm::mat4;

#include <vector>
using std::vector;

#include <unordered_map>
using std::unordered_map;

using namespace Rigid3D;

/**
 * @brief Demo Instructions
 *
 * Mesh Type:
 * Numbers 1 through 4 change the mesh type.
 *
 * Object Movement Keys:
 * a, s, d, w, r, f move the mesh object.
 */
class DepthMapping : public GlfwOpenGlWindow {

public:
    ~DepthMapping() { }

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    // Mesh and Batch Containers
    MeshConsolidator meshConsolidator;
    unordered_map<const char *, BatchInfo> batchInfoMap;

    // Matrices
    mat4 modelMatrix_grid;
    mat4 modelMatrix_bunny;
    mat4 modelMatrix_sphere;

    ShaderProgram shaderProgram;

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;

    DepthMapping(); // Singleton, prevent direct construction.

    virtual void init();
    virtual void logic();
    virtual void draw();
    virtual void keyInput(int key, int action, int mods);
    virtual void cleanup();

    void setupShaders();
    void setupGLBuffers();
    void setupMatrices();
    void updateMatrices();
    void updateUniformData();
    void drawWalls();
    void drawBunny();
    void drawSphere();
};

#endif /* DEPTH_MAPPING_HPP */
