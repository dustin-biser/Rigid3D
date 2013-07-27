#ifndef LOADMESHOBJ_EXAMPLE_HPP_
#define LOADMESHOBJ_EXAMPLE_HPP_

#include <GlUtils.hpp>
#include <SfmlOpenGLWindow.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <vector>

using namespace GlUtils;
using glm::mat4;

class LoadMeshObj_Example : public SfmlOpenGLWindow {

public:
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
    GLuint worldToCamera_Location;
    GLuint cameraToClip_Location;
    GLuint modelToWorld_Location;

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_indices;

    void setupGl();
    void setupShaders();
    void setupMatrices();
    void setupVertexArrayObject();

};

#endif /* LOADMESHOBJ_EXAMPLE_HPP_ */
