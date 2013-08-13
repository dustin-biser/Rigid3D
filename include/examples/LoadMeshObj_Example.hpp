#ifndef LOADMESHOBJ_EXAMPLE_HPP_
#define LOADMESHOBJ_EXAMPLE_HPP_

#include <GlfwOpenGlWindow.hpp>
#include <GlUtils.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <vector>

using namespace GlUtils;
using glm::mat4;

class LoadMeshObj_Example : public GlfwOpenGlWindow {

public:
    LoadMeshObj_Example() { }

    ~LoadMeshObj_Example() { }

    void setupGLBuffers();

    virtual void init();

    virtual void logic();

    virtual void draw();

    virtual void resize(int width, int height);

    virtual void cleanup();

protected:
    ShaderProgram shaderProgram;
    Mesh mesh;
    Frustum frustum;
    mat4 modelToWorldMatrix;
    mat4 worldToCameraMatrix;
    mat4 cameraToClipMatrix;

    // Vertex attribute locations
    GLint position_AttribLocation;
    GLint color_Location;
    GLint normal_AttribLocation;
    GLint worldToCamera_Location;
    GLint cameraToClip_Location;
    GLint modelToWorld_Location;

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_indices;

    void setupGl() { }
    void setupShaders();
    void setupMatrices();
    void setupVertexArrayObject() { }

};

#endif /* LOADMESHOBJ_EXAMPLE_HPP_ */
