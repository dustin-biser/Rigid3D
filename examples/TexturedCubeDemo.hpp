/**
 * @brief TexturedCubeDemo Demo
 *
 * @author Dustin Biser
 */
#include "GlfwOpenGlWindow.hpp"
#include <glm/glm.hpp>
using glm::mat4;
using glm::mat3;

#include "Rigid3D.hpp"
using namespace Rigid3D;

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

    TexturedCubeDemo(); // Singleton, prevent direct construction.

    virtual void init();
    virtual void reloadShaderProgram();
    virtual void logic();
    virtual void draw();
    virtual void setupGl();
    virtual void cleanup();

    void setupShaders();
    void setupMatrices();
    void setupUniformData();
    void setupVertexData();
    void setupTextureData();
};
