#ifndef FLAT_SMOOTH_SHADING_EXAMPLE_HPP
#define FLAT_SMOOTH_SHADING_EXAMPLE_HPP

#include <GlfwOpenGlWindow.hpp>
#include <GlUtils.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <vector>
#include <memory>

using namespace GlUtils;
using namespace glm;
using namespace std;

/**
 * @brief Demo Instructions
 *
 * Shading Type:
 * Press F1 for flat shading.
 * Press F2 for smooth shading.
 *
 * Mesh Type:
 * Numbers 1 through 4 change the mesh type.
 *
 * Object Movement Keys:
 * a, s, d, w, r, f move the mesh object.
 *
 * Light Movement Keys:
 * up, down, left, right, p, l move the light source.
 */

class FlatSmoothShading_Example : public GlfwOpenGlWindow {

public:
    ~FlatSmoothShading_Example() { }

    virtual void resize(int width, int height);

    virtual void cleanup();

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    // Mesh Objects
    shared_ptr<Mesh> susanMeshFlat;
    shared_ptr<Mesh> sphereMeshFlat;
    shared_ptr<Mesh> cubeMeshFlat;
    shared_ptr<Mesh> torusMeshFlat;
    shared_ptr<Mesh> susanMeshSmooth;
    shared_ptr<Mesh> sphereMeshSmooth;
    shared_ptr<Mesh> cubeMeshSmooth;
    shared_ptr<Mesh> torusMeshSmooth;
    shared_ptr<MeshConsolidator> meshConsolidator;

    vector<BatchInfo> batchInfoVec;


    enum class MeshType {CUBE, SPHERE, TORUS, SUSAN};
    enum class ShadingType {FLAT, SMOOTH};

    MeshType renderTarget;
    ShadingType shadingType;

    // Shader Program Data
    ShaderProgram shaderProgram;
    Frustum frustum;
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 modelViewMatrix;
    mat4 projectionMatrix;
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

    FlatSmoothShading_Example();

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

#endif /* FLAT_SMOOTH_SHADING_EXAMPLE_HPP */
