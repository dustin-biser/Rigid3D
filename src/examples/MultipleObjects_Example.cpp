#include <MultipleObjects_Example.hpp>
#include <MathUtils.hpp>
#include <GlErrorCheck.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <memory>

using namespace std;
using namespace MathUtils;

int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = MultipleObjects_Example::getInstance();
    meshDemo->create(800, 600, "Load Mesh Object Example");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> MultipleObjects_Example::getInstance() {
    static GlfwOpenGlWindow * instance = new MultipleObjects_Example();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
MultipleObjects_Example::MultipleObjects_Example() {
    // Set light and material properties for all objects.
    lightPositionEC = vec3(0.0f, 0.0f, 5.0f);
    Ia = vec3(0.2f, 0.2f, 0.2f);
    Id = vec3(0.6f, 0.2f, 0.8f);
    Ka = vec3(1.0f, 1.0f, 1.0f);
    Kd = vec3(1.0f, 1.0f, 1.0f);

    renderTarget = MeshType::CUBE;
}

//---------------------------------------------------------------------------------------
void MultipleObjects_Example::setupGLBuffers()
{
    //-- Concatenate vertex data from all meshes.
    size_t totalVertexBytes = cubeMesh.getNumVertexBytes() + sphereMesh.getNumVertexBytes() +
                              torusMesh.getNumVertexBytes() + susanMesh.getNumVertexBytes();

    float * vertexDataPtr = (float *)malloc(totalVertexBytes);
    float * data = vertexDataPtr;
    memcpy(data, cubeMesh.getVertexDataPtr(), cubeMesh.getNumVertexBytes());
    data += cubeMesh.getNumVertexBytes() / sizeof(float);
    memcpy(data, sphereMesh.getVertexDataPtr(), sphereMesh.getNumVertexBytes());
    data += sphereMesh.getNumVertexBytes() / sizeof(float);
    memcpy(data, torusMesh.getVertexDataPtr(), torusMesh.getNumVertexBytes());
    data += torusMesh.getNumVertexBytes() / sizeof(float);
    memcpy(data, susanMesh.getVertexDataPtr(), susanMesh.getNumVertexBytes());
    data += susanMesh.getNumVertexBytes() / sizeof(float);

    //-- Concatenate normal data from all meshes.
    size_t totalNormalBytes = cubeMesh.getNumNormalBytes() + sphereMesh.getNumNormalBytes() +
                              torusMesh.getNumNormalBytes() + susanMesh.getNumNormalBytes();

    float * normalDataPtr = (float *)malloc(totalNormalBytes);
    data = normalDataPtr;
    memcpy(data, cubeMesh.getNormalDataPtr(), cubeMesh.getNumNormalBytes());
    data += cubeMesh.getNumNormalBytes() / sizeof(float);
    memcpy(data, sphereMesh.getNormalDataPtr(), sphereMesh.getNumNormalBytes());
    data += sphereMesh.getNumNormalBytes() / sizeof(float);
    memcpy(data, torusMesh.getNormalDataPtr(), torusMesh.getNumNormalBytes());
    data += torusMesh.getNumNormalBytes() / sizeof(float);
    memcpy(data, susanMesh.getNormalDataPtr(), susanMesh.getNumNormalBytes());
    data += susanMesh.getNumNormalBytes() / sizeof(float);


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
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void MultipleObjects_Example::init()
{
    susanMesh.fromObjFile("../data/susan.obj");
    sphereMesh.fromObjFile("../data/sphere.obj");
    cubeMesh.fromObjFile("../data/cube2.obj");
    torusMesh.fromObjFile("../data/torus.obj");


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    setupShaders();
    setupGLBuffers();
    setupMatrices();
}

//---------------------------------------------------------------------------------------
void MultipleObjects_Example::setupShaders() {
    shaderProgram.loadFromFile("../data/AmbientDiffuseLighting.vert", "../data/AmbientDiffuseLighting.frag");

    // Acquire vertex attribute locations.
    position_AttribLocation = shaderProgram.getAttribLocation("vertexPosition");
    normal_AttribLocation = shaderProgram.getAttribLocation("vertexNormal");
    projectionMatrix_UniformLoc = shaderProgram.getUniformLocation("ProjectionMatrix");
    modelViewMatrix_UniformLoc = shaderProgram.getUniformLocation("ModelViewMatrix");
    normalMatrix_UniformLoc = shaderProgram.getUniformLocation("NormalMatrix");

    lightPositionEC_UniformLocation = shaderProgram.getUniformLocation("lightPositionEC");
    GLint Ka_UniformLocation = shaderProgram.getUniformLocation("Ka");
    GLint Kd_UniformLocation = shaderProgram.getUniformLocation("Kd");
    GLint Ia_UniformLocation = shaderProgram.getUniformLocation("Ia");
    GLint Id_UniformLocation = shaderProgram.getUniformLocation("Id");

    shaderProgram.enable();
        // Enable vertex attribute arrays so we can send position data to vertex shader.
        glEnableVertexAttribArray(position_AttribLocation);
        // Enable normal attribute arrays so we can send normal data to vertex shader.
        glEnableVertexAttribArray(normal_AttribLocation);

        // Pass in uniform data
        glUniform3fv(lightPositionEC_UniformLocation, 1, glm::value_ptr(lightPositionEC));
        glUniform3fv(Ka_UniformLocation, 1, glm::value_ptr(Ka));
        glUniform3fv(Kd_UniformLocation, 1, glm::value_ptr(Kd));
        glUniform3fv(Ia_UniformLocation, 1, glm::value_ptr(Ia));
        glUniform3fv(Id_UniformLocation, 1, glm::value_ptr(Id));
    shaderProgram.disable();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void MultipleObjects_Example::setupMatrices() {
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
void MultipleObjects_Example::draw()
{
    static const GLint cubeStartIndex = 0;
    static const GLint sphereStartIndex = cubeMesh.getNumVertices();
    static const GLint torusStartIndex = sphereStartIndex + sphereMesh.getNumVertices();
    static const GLint susanStartIndex = torusStartIndex + torusMesh.getNumVertices();

    shaderProgram.enable();
    switch (renderTarget) {
    case (MeshType::CUBE):
        glDrawArrays(GL_TRIANGLES, cubeStartIndex, cubeMesh.getNumVertices());
        break;
    case (MeshType::SPHERE):
        glDrawArrays(GL_TRIANGLES, sphereStartIndex, sphereMesh.getNumVertices());
        break;
    case (MeshType::TORUS):
        glDrawArrays(GL_TRIANGLES, torusStartIndex, torusMesh.getNumVertices());
        break;
    case (MeshType::SUSAN):
        glDrawArrays(GL_TRIANGLES, susanStartIndex, susanMesh.getNumVertices());
        break;
    }
    shaderProgram.disable();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void MultipleObjects_Example::resize(int width, int height)
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
void MultipleObjects_Example::logic() {
    updateMatrices();
    updateUniformData();
}

//---------------------------------------------------------------------------------------
void MultipleObjects_Example::updateMatrices() {
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
void MultipleObjects_Example::updateUniformData() {
    shaderProgram.enable();
        // Pass in uniform data
        glUniform3fv(lightPositionEC_UniformLocation, 1, glm::value_ptr(lightPositionEC));
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void MultipleObjects_Example::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}

//---------------------------------------------------------------------------------------
void MultipleObjects_Example::keyInput(int key, int scancode, int action, int mods) {
    GlfwOpenGlWindow::keyInput(key, scancode, action, mods);
    static const float xDelta = 0.5f;
    static const float yDelta = 0.2f;
    static const float zDelta = 0.5f;

    // Select Render Target
    if (key == GLFW_KEY_1) {
        renderTarget = MeshType::CUBE;
    } else if (key == GLFW_KEY_2) {
        renderTarget = MeshType::SPHERE;
    } else if (key == GLFW_KEY_3) {
        renderTarget = MeshType::TORUS;
    } else if (key == GLFW_KEY_4) {
        renderTarget = MeshType::SUSAN;
    }

    // Light Source Movement
    if (key == GLFW_KEY_LEFT) {
        lightPositionEC += vec3(-1 * xDelta, 0.0f, 0.0f);
    } else if (key == GLFW_KEY_RIGHT) {
        lightPositionEC += vec3(xDelta, 0.0f, 0.0f);
    } else if (key == GLFW_KEY_UP) {
        lightPositionEC += vec3(0.0f, 0.0f, -1 * zDelta);
    } else if (key == GLFW_KEY_DOWN) {
        lightPositionEC += vec3(0.0f, 0.0f, zDelta);
    }

    // Object Movement
    if (key == GLFW_KEY_A) {
        modelToWorldMatrix = translate(modelToWorldMatrix, -1 * xDelta, 0.0f, 0.0f);
    } else if (key == GLFW_KEY_D) {
        modelToWorldMatrix = translate(modelToWorldMatrix, xDelta, 0.0f, 0.0f);
    } else if (key == GLFW_KEY_W) {
        modelToWorldMatrix = translate(modelToWorldMatrix, 0.0f, yDelta, 0.0f);
    } else if (key == GLFW_KEY_S) {
        modelToWorldMatrix = translate(modelToWorldMatrix, 0.0f, -1 * yDelta, 0.0f);
    } else if (key == GLFW_KEY_R) {
        modelToWorldMatrix = translate(modelToWorldMatrix, 0.0f, 0.0f, -1 * zDelta);
    } else if (key == GLFW_KEY_F) {
        modelToWorldMatrix = translate(modelToWorldMatrix, 0.0f, 0.0f, zDelta);
    }

    // TODO Remove debug statement.
    cout << "lightPositionEC = " << lightPositionEC << endl << endl;
}
