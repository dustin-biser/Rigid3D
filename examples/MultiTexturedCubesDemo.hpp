/**
* @brief MultiTexturedCubes
*
* @author Dustin Biser
*/
#include "Utils/GlfwOpenGlWindow.hpp"
#include "Rigid3D/Rigid3D.hpp"

using namespace Rigid3D;

class MultiTexturedCubesDemo : public GlfwOpenGlWindow {

public:
    ~MultiTexturedCubesDemo() { }

    static std::shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    mat4 modelMatrixA;
    mat4 modelMatrixB;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    mat3 normalMatrix;

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_textureCoords;
    GLuint vbo_indices;
    GLuint numIndices;

    GLuint textureId;

    Mesh texturedCubeA;
    Mesh texturedCubeB;

    // World space vertex positions for the 2 Cubes.
    std::unique_ptr<float32[]> positionDataPtr;

    ShaderProgram shader;

    MultiTexturedCubesDemo(); // Singleton, prevent direct construction.

    virtual void init();
    virtual void reloadShaderProgram();
    virtual void logic();
    virtual void draw();
    virtual void setupGl();
    virtual void cleanup();

    void setupShaders();
    void setupUniformData();
    void setupVboData();
    void setupVertexAttributeMapping();
    void setupTextureData();
    void transformVertexDataToWorldSpace();
};
