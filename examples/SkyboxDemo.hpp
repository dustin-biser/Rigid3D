/**
* @brief SkyboxDemo
*
* @author Dustin Biser
*/
#include "Utils/GlfwOpenGlWindow.hpp"
#include "Rigid3D/Rigid3D.hpp"

using namespace Rigid3D;

class SkyBoxDemo : public GlfwOpenGlWindow {

public:
    ~SkyBoxDemo() { }

    static std::shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat3 normalMatrix;

    GLuint vao_cube;
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_textureCoords;

    GLuint cubeTexture;
    GLuint skyboxTexture;

    GLuint vao_skybox;
    GLuint vbo_skybox_vertices;

    Mesh texturedCube;
    ShaderProgram cubeShader;
    ShaderProgram skyboxShader;

    SkyBoxDemo(); // Singleton, prevent direct construction.

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
    void setupSkyBoxVertexData();
    void setupTextureData();
};
