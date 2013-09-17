#ifndef MESH_NORMALS_HPP
#define MESH_NORMALS_HPP

#include <GlfwOpenGlWindow.hpp>
#include <GlUtils.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <vector>

using namespace GlUtils;
using namespace glm;

class MeshNormals : public GlfwOpenGlWindow {

public:
    ~MeshNormals() { }

    virtual void resize(int width, int height);

    virtual void cleanup();

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    Mesh meshFlat, meshSmooth;

    enum class MeshType {FLAT, SMOOTH};
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

    GLuint vao;
    GLuint vbo_vertices;
    GLuint vbo_normals;

    MeshNormals();

    virtual void init();
    virtual void logic();
    virtual void draw();
    virtual void keyInput(int key, int scancode, int action, int mods);

    void setupGLBuffers();
    void setupShaders();
    void setupMatrices();
    void updateMatrices();
};

#endif /* MESH_NORMALS_HPP */
