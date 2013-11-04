/**
 * @brief TexturedCubeDemo Demo
 *
 * @author Dustin Biser
 */
#include "GlfwOpenGlWindow.hpp"
#include <glm/glm.hpp>
using glm::mat4;
using glm::mat3;

#include "Mesh.hpp"
using GlUtils::Mesh;

#include "ShaderProgram.hpp"
using GlUtils::ShaderProgram;

class TexturedCubeDemo : public GlfwOpenGlWindow {

public:
    ~TexturedCubeDemo() { }

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat3 normalMatrix;

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_textureCoords;
    GLuint textureId;

    Mesh texturedCube;
    ShaderProgram shader;

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

    TexturedCubeDemo(); // Singleton, prevent direct construction.

    virtual void init();
    virtual void reloadShaderProgram() { }
    virtual void logic();
    virtual void draw();
    virtual void setupGl();
    virtual void cleanup();
    virtual void keyInput(int key, int scancode, int action, int mods);

    void setupShaders();
    void setupMatrices();
    void setupVertexData();
    void setupTextureData();
    void processKeyInput();
};
