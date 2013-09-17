#include <MeshNormals.hpp>
#include <MathUtils.hpp>
#include <GlErrorCheck.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <memory>
#include <ctime>
#include <array>

using namespace std;
using namespace MathUtils;

int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = MeshNormals::getInstance();
    meshDemo->create(800, 600, "Load Multiple Mesh Objects Example");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> MeshNormals::getInstance() {
    static GlfwOpenGlWindow * instance = new MeshNormals();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
MeshNormals::MeshNormals() : renderTarget(MeshType::FLAT) {

}
//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void MeshNormals::init()
{
    meshFlat.fromObjFile("../data/meshes/sphere.obj");
    meshSmooth.fromObjFile("../data/meshes/sphere_smooth.obj");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    setupShaders();
    setupGLBuffers();
    setupMatrices();

    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
}
//---------------------------------------------------------------------------------------

void MeshNormals::setupGLBuffers()
{
    //-- Concatenate vertex data from all meshes.
    size_t totalVertexBytes = meshFlat.getNumVertexBytes() + meshSmooth.getNumVertexBytes();

    float * vertexDataPtr = (float *)malloc(totalVertexBytes);
    float * data = vertexDataPtr;
    memcpy(data, meshFlat.getVertexDataPtr(), meshFlat.getNumVertexBytes());
    data += meshFlat.getNumVertexBytes() / sizeof(float);
    memcpy(data, meshSmooth.getVertexDataPtr(), meshSmooth.getNumVertexBytes());

    //-- Concatenate normal data from all meshFlates.
    size_t totalNormalBytes = meshFlat.getNumNormalBytes() + meshSmooth.getNumNormalBytes();

    float * normalDataPtr = (float *)malloc(totalNormalBytes);
    data = normalDataPtr;
    memcpy(data, meshFlat.getNormalDataPtr(), meshFlat.getNumNormalBytes());
    data += meshFlat.getNumNormalBytes() / sizeof(float);
    memcpy(data, meshSmooth.getNormalDataPtr(), meshSmooth.getNumNormalBytes());

    // Register vertex positions with OpenGL
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, totalVertexBytes, vertexDataPtr, GL_STATIC_DRAW);
    glVertexAttribPointer(position_AttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Register normals with OpenGL
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, totalNormalBytes, normalDataPtr, GL_STATIC_DRAW);
    glVertexAttribPointer(normal_AttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete vertexDataPtr; vertexDataPtr = nullptr;
    delete normalDataPtr; normalDataPtr = nullptr;
    data = nullptr;
    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void MeshNormals::setupShaders() {
    shaderProgram.loadFromFile("../data/shaders/PerFragLighting.vert",
                               "../data/shaders/TestNormals.frag");

    // Acquire vertex attribute locations.
    position_AttribLocation = shaderProgram.getAttribLocation("vertexPosition");
    normal_AttribLocation = shaderProgram.getAttribLocation("vertexNormal");
    projectionMatrix_UniformLoc = shaderProgram.getUniformLocation("ProjectionMatrix");
    modelViewMatrix_UniformLoc = shaderProgram.getUniformLocation("ModelViewMatrix");
    normalMatrix_UniformLoc = shaderProgram.getUniformLocation("NormalMatrix");

    shaderProgram.enable();
        // Enable vertex attribute arrays so we can send position data to vertex shader.
        glEnableVertexAttribArray(position_AttribLocation);
        // Enable normal attribute arrays so we can send normal data to vertex shader.
        glEnableVertexAttribArray(normal_AttribLocation);
    shaderProgram.disable();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void MeshNormals::setupMatrices() {
    frustum = Frustum(45.0f, 4.0f/3.0f, 1.0f, 100.0f);
    cameraToClipMatrix = frustum.getPerspectiveMatrix();

    worldToCameraMatrix = glm::lookAt(glm::vec3(0.0f, 0.0, 5.0),
                                      glm::vec3(0.0 , 0.0, -1.0),
                                      glm::vec3(0.0, 1.0, 0.0));

    modelToWorldMatrix = glm::mat4(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, 1, 0,
                                   0, 0, 0, 1);

    // Translate the object into view.
    modelToWorldMatrix = translate(modelToWorldMatrix, 0.0f, 0.0f, -0.3f);

    modelViewMatrix = worldToCameraMatrix * modelToWorldMatrix;
    normalMatrix = mat3(modelViewMatrix);

    shaderProgram.enable();
        // Register uniform matrix data to vertex shader
        glUniformMatrix4fv(projectionMatrix_UniformLoc, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
        glUniformMatrix4fv(modelViewMatrix_UniformLoc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix3fv(normalMatrix_UniformLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void MeshNormals::draw()
{
    static const unsigned int flatMeshStartIndex = 0;
    static const unsigned int smoothMeshStartIndex = meshFlat.getNumVertices();

    shaderProgram.enable();
    switch(renderTarget){
    case MeshType::FLAT:
        glDrawArrays(GL_TRIANGLES, flatMeshStartIndex, meshFlat.getNumVertices());
        break;
    case MeshType::SMOOTH:
        glDrawArrays(GL_TRIANGLES, smoothMeshStartIndex, meshSmooth.getNumVertices());
        break;
    }
    shaderProgram.disable();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void MeshNormals::resize(int width, int height)
{
    float aspectRatio = ((float) width) / height;
    float frustumYScale = cotangent(degreesToRadians(frustum.getFieldOfViewY() / 2));

    float frustumXScale = frustumYScale * frustum.getAspectRatio();

    if (width > height) {
        // Shrink the x scale in eye-coordinate space, so that when geometry is
        // projected to ndc-space, it is widened out to become square.
        cameraToClipMatrix[0][0] = frustumXScale / aspectRatio;
        cameraToClipMatrix[1][1] = frustumYScale;
    }
    else {
        // Shrink the y scale in eye-coordinate space, so that when geometry is
        // projected to ndc-space, it is widened out to become square.
        cameraToClipMatrix[0][0] = frustumXScale;
        cameraToClipMatrix[1][1] = frustumYScale * aspectRatio;
    }

    // Use entire window for rendering.
    glViewport(0, 0, width, height);
}

//---------------------------------------------------------------------------------------
void MeshNormals::logic() {
    updateMatrices();
}

//---------------------------------------------------------------------------------------
void MeshNormals::updateMatrices() {
    modelViewMatrix = worldToCameraMatrix * modelToWorldMatrix;
    normalMatrix = mat3(modelViewMatrix);

    // Pass updated matrix data to vertex shader
    shaderProgram.enable();
        glUniformMatrix4fv(projectionMatrix_UniformLoc, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
        glUniformMatrix4fv(modelViewMatrix_UniformLoc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix3fv(normalMatrix_UniformLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    shaderProgram.disable();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void MeshNormals::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}

//---------------------------------------------------------------------------------------
void MeshNormals::keyInput(int key, int scancode, int action, int mods) {
    GlfwOpenGlWindow::keyInput(key, scancode, action, mods);
    static const float xDelta = 0.5f;
    static const float yDelta = 0.2f;
    static const float zDelta = 0.5f;

    // Object Movement
    if ((key == GLFW_KEY_A) && (action == GLFW_PRESS)) {
        modelToWorldMatrix = translate(modelToWorldMatrix, -1 * xDelta, 0.0f, 0.0f);
    } else if ((key == GLFW_KEY_D) && (action == GLFW_PRESS)) {
        modelToWorldMatrix = translate(modelToWorldMatrix, xDelta, 0.0f, 0.0f);
    } else if ((key == GLFW_KEY_W) && (action == GLFW_PRESS)) {
        modelToWorldMatrix = translate(modelToWorldMatrix, 0.0f, yDelta, 0.0f);
    } else if ((key == GLFW_KEY_S) && (action == GLFW_PRESS)) {
        modelToWorldMatrix = translate(modelToWorldMatrix, 0.0f, -1 * yDelta, 0.0f);
    } else if ((key == GLFW_KEY_R) && (action == GLFW_PRESS)) {
        modelToWorldMatrix = translate(modelToWorldMatrix, 0.0f, 0.0f, -1 * zDelta);
    } else if ((key == GLFW_KEY_F) && (action == GLFW_PRESS)) {
        modelToWorldMatrix = translate(modelToWorldMatrix, 0.0f, 0.0f, zDelta);
    }

    // Change Render Target
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_1) {
            renderTarget = MeshType::FLAT;
            cout << "Flat Shading" << endl;
        } else if (key == GLFW_KEY_2) {
            renderTarget = MeshType::SMOOTH;
            cout << "Smooth Shading" << endl;
        }
    }
}
