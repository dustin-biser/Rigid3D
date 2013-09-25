#ifndef MULTIPLE_OBJECTS_HPP
#define MULTIPLE_OBJECTS_HPP

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
class MultipleObjects : public GlfwOpenGlWindow {

public:
    ~MultipleObjects() { }

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    // Mesh and Batch Containers
    MeshConsolidator meshConsolidator;
    vector<BatchInfo> batchInfoVec;

    // Matrices
    mat4 modelMatrix_grid;
    mat4 modelMatrix_bunny;
    mat4 modelMatrix_tyrannosaurus;
    mat4 modelMatrix_sphere;
    mat4 modelMatrix_light;
    mat4 viewMatrix;
    mat3 normalMatrix;

    ShaderProgram shaderProgram;

    struct LightSource {
        vec3 position;      // Light position in world space.
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
    MaterialProperties material_grid;
    MaterialProperties material_bunny;
    MaterialProperties material_tyrannosaurus;
    MaterialProperties material_sphere;
    MaterialProperties material_light;

    bool pauseLightSource = false;

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;

    MultipleObjects(); // Singleton, prevent direct construction.

    virtual void init();
    virtual void logic();
    virtual void draw();
    virtual void keyInput(int key, int scancode, int action, int mods);
    virtual void cleanup();

    void setupShaders();
    void setupGLBuffers();
    void setupMatrices();
    void updateMatrices();
    void updateUniformData();
    void rotateLightSource();
    void drawGrid();
    void drawBunny();
    void drawTyrannosaurus();
    void drawSphere();
    void drawLight();
};

#endif /* MULTIPLE_OBJECTS_HPP */
