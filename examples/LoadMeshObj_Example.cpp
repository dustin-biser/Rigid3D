#include <LoadMeshObj_Example.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <memory>

using Rigid3D::cotangent;
using Rigid3D::degreesToRadians;

using namespace std;

int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = LoadMeshObj_Example::getInstance();
    meshDemo->create(800, 600, "Load Mesh Object Example");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> LoadMeshObj_Example::getInstance() {
    static GlfwOpenGlWindow * instance = new LoadMeshObj_Example();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
LoadMeshObj_Example::LoadMeshObj_Example() {
    lightPositionEC = vec3(0.0f, 0.0f, 5.0f);
    Kd = vec3(1.0f, 1.0f, 1.0f);
    Ld = vec3(0.6f, 0.2f, 0.8f);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupGLBuffers()
{
    // Register vertex positions with OpenGL
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, mesh.getNumVertexPositionBytes(), mesh.getVertexPositionDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(position_AttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Register normals with OpenGL
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, mesh.getNumVertexNormalBytes(), mesh.getVertexNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(normal_AttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    Rigid3D::checkGlErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void LoadMeshObj_Example::init()
{
    mesh.fromObjFile("../data/meshes/susan.obj");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    setupShaders();
    setupGLBuffers();
    setupMatrices();
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupShaders() {
    shaderProgram.loadFromFile("../data/shaders/DiffuseLighting.vert",
                               "../data/shaders/UniformVertexColors.frag");

    // Acquire vertex attribute locations.
    position_AttribLocation = shaderProgram.getAttribLocation("vertexPosition");
    normal_AttribLocation = shaderProgram.getAttribLocation("vertexNormal");
    projectionMatrix_UniformLoc = shaderProgram.getUniformLocation("ProjectionMatrix");
    modelViewMatrix_UniformLoc = shaderProgram.getUniformLocation("ModelViewMatrix");
    normalMatrix_UniformLoc = shaderProgram.getUniformLocation("NormalMatrix");

    lightPositionEC_UniformLocation = shaderProgram.getUniformLocation("lightPositionEC");
    GLint Kd_UniformLocation = shaderProgram.getUniformLocation("Kd");
    GLint Ld_UniformLocation = shaderProgram.getUniformLocation("Ld");

    shaderProgram.enable();
        // Enable vertex attribute arrays so we can send position data to vertex shader.
        glEnableVertexAttribArray(position_AttribLocation);
        // Enable normal attribute arrays so we can send normal data to vertex shader.
        glEnableVertexAttribArray(normal_AttribLocation);

        // Pass in uniform data
        glUniform3fv(lightPositionEC_UniformLocation, 1, glm::value_ptr(lightPositionEC));
        glUniform3fv(Kd_UniformLocation, 1, glm::value_ptr(Kd));
        glUniform3fv(Ld_UniformLocation, 1, glm::value_ptr(Ld));
    shaderProgram.disable();

    Rigid3D::checkGlErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupMatrices() {
    frustum = Frustum(45.0f, 4.0f/3.0f, 1.0f, 100.0f);
    cameraToClipMatrix = frustum.getProjectionMatrix();

    worldToCameraMatrix = glm::lookAt(glm::vec3(0.0f, 0.0, 5.0),
                                      glm::vec3(0.0 , 0.0, -1.0),
                                      glm::vec3(0.0, 1.0, 0.0));

    modelToWorldMatrix = glm::mat4(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, 1, 0,
                                   0, 0, 0, 1);

    // Translate the object into view.
    modelToWorldMatrix = translate(modelToWorldMatrix, vec3(-1.0f, 1.0f, -2.0f));

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
void LoadMeshObj_Example::draw()
{
    shaderProgram.enable();
        glDrawArrays(GL_TRIANGLES, 0, mesh.getNumVertexPositions());
    shaderProgram.disable();

    Rigid3D::checkGlErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::resize(int width, int height)
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
void LoadMeshObj_Example::logic() {
    updateMatrices();
    updateUniformData();
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::updateMatrices() {
    modelViewMatrix = worldToCameraMatrix * modelToWorldMatrix;
    normalMatrix = mat3(modelViewMatrix);

    // Pass updated matrix data to vertex shader
    shaderProgram.enable();
        glUniformMatrix4fv(projectionMatrix_UniformLoc, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
        glUniformMatrix4fv(modelViewMatrix_UniformLoc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glUniformMatrix3fv(normalMatrix_UniformLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    shaderProgram.disable();

    Rigid3D::checkGlErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::updateUniformData() {
    shaderProgram.enable();
        // Pass in uniform data
        glUniform3fv(lightPositionEC_UniformLocation, 1, glm::value_ptr(lightPositionEC));
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::keyInput(int key, int action, int mods) {
    static const float xDelta = 0.5f;
    static const float yDelta = 0.2f;
    static const float zDelta = 0.5f;

    // Light Source Movement
    if (key == GLFW_KEY_LEFT) {
       lightPositionEC += vec3(-1*xDelta, 0.0f, 0.0f);
    }
    else if (key == GLFW_KEY_RIGHT) {
       lightPositionEC += vec3(xDelta, 0.0f, 0.0f);
    }
    else if (key == GLFW_KEY_UP) {
       lightPositionEC += vec3(0.0f, 0.0f, -1*zDelta);
    }
    else if (key == GLFW_KEY_DOWN) {
       lightPositionEC += vec3(0.0f, 0.0f, zDelta);
    }

    // Object Movement
    if (key == GLFW_KEY_A) {
        modelToWorldMatrix = translate(modelToWorldMatrix, vec3(-1*xDelta, 0.0f, 0.0f));
    }
    else if (key == GLFW_KEY_D) {
        modelToWorldMatrix = translate(modelToWorldMatrix, vec3(xDelta, 0.0f, 0.0f));
    }
    else if (key == GLFW_KEY_W) {
        modelToWorldMatrix = translate(modelToWorldMatrix, vec3(0.0f, yDelta, 0.0f));
    }
    else if (key == GLFW_KEY_S) {
        modelToWorldMatrix = translate(modelToWorldMatrix, vec3(0.0f, -1*yDelta, 0.0f));
    }
    else if (key == GLFW_KEY_R) {
        modelToWorldMatrix = translate(modelToWorldMatrix, vec3(0.0f, 0.0f, -1*zDelta));
    }
    else if (key == GLFW_KEY_F) {
        modelToWorldMatrix = translate(modelToWorldMatrix, vec3(0.0f, 0.0f, zDelta));
    }

    // TODO Remove debug statement.
    cout << "lightPositionEC = " << lightPositionEC << endl << endl;
}
