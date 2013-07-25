#ifndef LOADMESHOBJ_EXAMPLE_HPP_
#define LOADMESHOBJ_EXAMPLE_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <Mesh.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <vector>
#include <MathUtils.hpp>
#include <Frustum.hpp>
#include <SfmlOpenGLWindow.hpp>

using GlUtils::Mesh;
using GlUtils::Frustum;
using glm::mat4;

class LoadMeshObj_Example : public SfmlOpenGLWindow {

public:
    virtual void setupGL();

    virtual void init();

    virtual void logic();

    virtual void draw();

    virtual void resize(int width, int height);

    virtual void cleanup();


protected:
    ShaderProgram shaderProgram;
    Mesh mesh;
    Frustum frustum;
    mat4 worldToCameraMatrix;
    mat4 cameraToClipMatrix;

    // Vertex attribute locations
    GLint position_AttribLocation;
    GLint color_AttribLocation;
    GLint normal_AttribLocation;
    GLuint worldToCamera_Location;
    GLuint cameraToClip_Location;

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_indices;

    void setupGlBuffers();
    void setupShaders();
    void setupMatrices();
    void setupVertexArrayObject();

};

#endif /* LOADMESHOBJ_EXAMPLE_HPP_ */
