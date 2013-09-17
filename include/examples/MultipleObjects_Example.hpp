#ifndef MULITPLE_OBJECTS_EXAMPLE_HPP
#define MULITPLE_OBJECTS_EXAMPLE_HPP

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

    struct LightSource {
        vec3 position;      // Light position in eye coordinate space.
        vec3 rgbIntensity;  // Light intensity for each RGB component.
    };
    LightSource lightSource;

    struct MaterialProperties {
        vec3 emission;  // Emission light intensity from material for each RGB component.
        vec3 Ka;        // Coefficients of ambient reflectivity for each RGB component.
        vec3 Kd;        // Coefficients of diffuse reflectivity for each RGB component.
        float Ks;       // Coefficient of specular reflectivity, uniform across each RGB component.
        float shininessFactor;   // Specular shininess factor.
    };
    MaterialProperties material;

    bool pauseLightSource = false;

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
    void rotateLightSource();

};

#endif /* MULITPLE_OBJECTS_EXAMPLE_HPP */
