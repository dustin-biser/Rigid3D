/**
 * @brief CameraExample.hpp
 */

#ifndef CAMERAEXAMPLE_HPP_
#define CAMERAEXAMPLE_HPP_

#include <GlfwOpenGlWindow.hpp>
#include <MeshConsolidator.hpp>
#include <GlUtils.hpp>

#include <unordered_map>
using std::unordered_map;

using namespace GlUtils;

class CameraExample : public GlfwOpenGlWindow {

public:
    ~CameraExample() { }

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    // Mesh and Batch Containers
    MeshConsolidator meshConsolidator;
    unordered_map<const char *, BatchInfo> batchInfoMap;

    // Matrices
    mat4 modelMatrix_grid;
    mat4 modelMatrix_bunny;
    mat4 modelMatrix_tyrannosaurus;
    mat4 modelMatrix_sphere;
    mat4 modelMatrix_light;
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

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;

    // Key Input
    bool key_r_down = false;
    bool key_f_down = false;
    bool key_q_down = false;
    bool key_e_down = false;
    bool key_w_down = false;
    bool key_s_down = false;
    bool key_a_down = false;
    bool key_d_down = false;
    bool key_left_down = false;
    bool key_right_down = false;
    bool key_up_down = false;
    bool key_down_down = false;

    bool lookAt_bunny = false;
    bool lookAt_sphere = false;
    bool lookAt_tyrannosaurus = false;
    bool lookAt_light = false;

    CameraExample(); // Singleton, prevent direct construction.

    virtual void init();
    virtual void logic();
    virtual void draw();
    virtual void keyInput(int key, int action, int mods);
    virtual void mouseScroll(double xOffSet, double yOffSet);
    virtual void cleanup();

    void setupShaders();
    void setupGLBuffers();
    void setupMatrices();
    void updateMatrices();
    void updateUniformData();
    void drawGrid();
    void drawBunny();
    void drawTyrannosaurus();
    void drawSphere();
    void drawLight();
    void processKeyInput();
};



#endif /* CAMERAEXAMPLE_HPP_ */
