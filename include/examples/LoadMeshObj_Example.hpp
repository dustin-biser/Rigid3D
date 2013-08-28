#ifndef LOADMESHOBJ_EXAMPLE_HPP_
#define LOADMESHOBJ_EXAMPLE_HPP_

#include <GlfwOpenGlWindow.hpp>
#include <GlUtils.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <vector>

using namespace GlUtils;
using namespace glm;

class LoadMeshObj_Example : public GlfwOpenGlWindow {

public:
    ~LoadMeshObj_Example() { }

    virtual void resize(int width, int height);

    virtual void cleanup();

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    ShaderProgram shaderProgram;
    Mesh mesh;
    Frustum frustum;
    mat4 modelToWorldMatrix;
    mat4 worldToCameraMatrix;
    mat4 modelViewMatrix;
    mat4 cameraToClipMatrix;
    mat3 normalMatrix;

    // Vertex attribute locations
    GLint position_AttribLocation;
    GLint normal_AttribLocation;
    GLint projectionMatrix_UniformLoc;
    GLint modelViewMatrix_UniformLoc;
    GLint normalMatrix_UniformLoc;

    // Lighting Parameters
    vec3 lightPositionEC; // light position in Eye Coordinate Space
    vec3 Kd; // coefficient of diffuse reflectivity
    vec3 Ld; // light source diffuse intensity

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_indices;

    LoadMeshObj_Example();

    virtual void init();
    virtual void logic();
    virtual void draw();
    virtual void keyInput(int key, int scancode, int action, int mods);

    void setupGLBuffers();
    void setupShaders();
    void setupMatrices();
    void updateMatrices();

};

#endif /* LOADMESHOBJ_EXAMPLE_HPP_ */
