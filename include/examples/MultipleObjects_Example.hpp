#ifndef MULTIPLE_OBJECTS_EXAMPLE_HPP_
#define MULTIPLE_OBJECTS_EXAMPLE_HPP_

#include <GlfwOpenGlWindow.hpp>
#include <GlUtils.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <vector>

using namespace GlUtils;
using namespace glm;

class MultipleObjects_Example : public GlfwOpenGlWindow {

public:
    ~MultipleObjects_Example() { }

    virtual void resize(int width, int height);

    virtual void cleanup();

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    // Mesh Objects
    Mesh susanMesh;
    Mesh sphereMesh;
    Mesh cubeMesh;
    Mesh torusMesh;

    enum class MeshType {CUBE, SPHERE, TORUS, SUSAN};
    MeshType renderTarget;

    // Shader Program Data
    ShaderProgram shaderProgram;
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
    GLint lightPositionEC_UniformLocation;

    // Lighting Parameters
    vec3 lightPositionEC; // light position in Eye Coordinate Space
    vec3 Ia; // Ambient light intensity over RGB spectrum.
    vec3 Id; // Diffuse light intensity over RGB specturm.
    vec3 Kd; // coefficient of diffuse reflectivity
    vec3 Ka; // coefficient of ambient reflectivity

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;

    MultipleObjects_Example();

    virtual void init();
    virtual void logic();
    virtual void draw();
    virtual void keyInput(int key, int scancode, int action, int mods);

    void setupGLBuffers();
    void setupShaders();
    void setupMatrices();
    void updateMatrices();
    void updateUniformData();

};

#endif /* MULTIPLE_OBJECTS_EXAMPLE_HPP_ */
