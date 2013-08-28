#include <LoadMeshObj_Example.hpp>
#include <MathUtils.hpp>
#include <GlErrorCheck.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>

using namespace std;
using namespace MathUtils;

// TODO - Normals must be transformed differently than vertices
// when moving the camera.
// See: http://www.songho.ca/opengl/gl_normaltransform.html

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
    lightPositionEC = vec3(-15.0f, 0.0f, 5.0f);
    Kd = vec3(1.0f, 1.0f, 1.0f);
    Ld = vec3(0.3f, 0.02f, 0.4f);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupGLBuffers()
{
    // Register vertex data with OpenGL
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, mesh.getNumVertexBytes(), mesh.getVertexDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(position_AttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Register normals with OpenGL
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, mesh.getNumNormalBytes(), mesh.getNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(normal_AttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Register index elements with OpenGL
    glGenBuffers(1, &vbo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getNumIndexBytes(), mesh.getIndexDataPtr(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void LoadMeshObj_Example::init()
{
    mesh.fromObjFile("../data/triangles.obj");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    setupShaders();
    setupGLBuffers();
    setupMatrices();

}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupShaders() {
    shaderProgram.loadFromFile("../data/DiffuseLighting.vert", "../data/UniformVertexColors.frag");

    // Acquire vertex attribute locations.
    this->position_AttribLocation = shaderProgram.getAttribLocation("vertexPosition");
    this->normal_AttribLocation = shaderProgram.getAttribLocation("vertexNormal");
    this->projectionMatrix_UniformLoc = shaderProgram.getUniformLocation("ProjectionMatrix");
    this->modelViewMatrix_UniformLoc = shaderProgram.getUniformLocation("ModelViewMatrix");
    this->normalMatrix_UniformLoc = shaderProgram.getUniformLocation("NormalMatrix");

    GLint lightPositionEC_UniformLocation = shaderProgram.getUniformLocation("lightPositionEC");
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

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupMatrices() {
    frustum = Frustum(45.0f, 4.0f/3.0f, 1.0f, 100.0f);
    cameraToClipMatrix = frustum.getPerspectiveMatrix();

    worldToCameraMatrix = glm::lookAt(glm::vec3(0.0f, 0.0, 5.0),
                                      glm::vec3(0.0 , 0.0, -1.0),
                                      glm::vec3(0.0, 1.0, 0.0));

    modelToWorldMatrix = glm::mat4(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, 1, -5,
                                   0, 0, 0, 1);

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
        glDrawElements(GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_SHORT, nullptr);
    shaderProgram.disable();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
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

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::cleanup() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_indices);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::keyInput(int key, int scancode, int action, int mods) {
    GlfwOpenGlWindow::keyInput(key, scancode, action, mods);
    cout << "Key Pressed!!" << endl;
}
