#include <DepthMapping.hpp>
#include <GlErrorCheck.hpp>

#include <glm/gtx/transform.hpp>
using glm::translate;
using glm::scale;


int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = DepthMapping::getInstance();
    meshDemo->create(800, 600, "Rendering Multiple Objects");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> DepthMapping::getInstance() {
    static GlfwOpenGlWindow * instance = new DepthMapping();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
DepthMapping::DepthMapping()
        : vao(0), vbo_vertices(0), vbo_normals(0) {

}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void DepthMapping::init()
{
    meshConsolidator =  {"../data/meshes/shadow_box.obj",
                         "../data/meshes/bunny_smooth.obj",
                         "../data/meshes/sphere_smooth.obj"};

    meshConsolidator.getBatchInfo(batchInfoVec);

    setupShaders();
    setupGLBuffers();
    setupMatrices();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//---------------------------------------------------------------------------------------
void DepthMapping::setupShaders() {
    shaderProgram.loadFromFile("../data/shaders/DepthMap.vert",
                               "../data/shaders/DepthMap.frag");

    // Generate VAO and enable vertex attribute arrays for positions and normals.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLint position_Location = shaderProgram.getAttribLocation("vertexPosition");
    glEnableVertexAttribArray(position_Location);

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void DepthMapping::setupGLBuffers()
{
    // Register vertex positions with OpenGL within the context of the bound VAO.
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexBytes(), meshConsolidator.getVertexDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void DepthMapping::setupMatrices() {
    camera.setNearZDistance(5.5f);
    camera.setFarZDistance(20.0f);
    camera.lookAt(vec3(4.0f, 2.0, -5.0),
                  vec3(0.0f, -2.0f, -10.0f),
                  vec3(0.0, 1.0, 0.0));

    mat4 identity = mat4();

    modelMatrix_grid = translate(identity, vec3(2.0f, -3.8f, -8.0f));
    modelMatrix_bunny = translate(identity, vec3(0.2f, -3.6f, -11.5f));
    modelMatrix_sphere = translate(identity, vec3(-0.5f, -3.3f, -9.0f));

    shaderProgram.setUniform("ViewMatrix", camera.getViewMatrix());
    shaderProgram.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
}

//---------------------------------------------------------------------------------------
void DepthMapping::draw()
{
    drawGrid();
    drawBunny();
    drawSphere();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void DepthMapping::drawGrid() {
    shaderProgram.setUniform("ModelMatrix", modelMatrix_grid);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoVec.at(0).startIndex, batchInfoVec.at(0).numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void DepthMapping::drawBunny() {
    shaderProgram.setUniform("ModelMatrix", modelMatrix_bunny);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoVec.at(1).startIndex, batchInfoVec.at(1).numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void DepthMapping::drawSphere() {
    shaderProgram.setUniform("ModelMatrix", modelMatrix_sphere);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoVec.at(2).startIndex, batchInfoVec.at(2).numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void DepthMapping::logic() {
    updateMatrices();
    updateUniformData();
}

//---------------------------------------------------------------------------------------
void DepthMapping::updateMatrices() {
    shaderProgram.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
}

//---------------------------------------------------------------------------------------
void DepthMapping::updateUniformData() {
}

//---------------------------------------------------------------------------------------
void DepthMapping::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}

//---------------------------------------------------------------------------------------
void DepthMapping::keyInput(int key, int scancode, int action, int mods) {
    GlfwOpenGlWindow::keyInput(key, scancode, action, mods); // Handle escape to close window

    static const float xDelta = 0.5f;
    static const float yDelta = 0.2f;
    static const float zDelta = 0.5f;
    static mat4 *modelMatrix = &this->modelMatrix_grid;

    // Target Object
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_F1:
            modelMatrix = &this->modelMatrix_grid;
            break;
        case GLFW_KEY_F2:
            modelMatrix = &this->modelMatrix_bunny;
            break;
        case GLFW_KEY_F3:
            modelMatrix = &this->modelMatrix_sphere;
            break;
        default:
            break;
        }
    }

    // Object Movement
    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
        if (key == GLFW_KEY_A) {
            *modelMatrix = translate(*modelMatrix, vec3(-1 * xDelta, 0.0f, 0.0f));
        } else if (key == GLFW_KEY_D) {
            *modelMatrix = translate(*modelMatrix, vec3(xDelta, 0.0f, 0.0f));
        } else if (key == GLFW_KEY_W) {
            *modelMatrix = translate(*modelMatrix, vec3(0.0f, yDelta, 0.0f));
        } else if (key == GLFW_KEY_S) {
            *modelMatrix = translate(*modelMatrix, vec3(0.0f, -1 * yDelta, 0.0f));
        } else if (key == GLFW_KEY_R) {
            *modelMatrix = translate(*modelMatrix, vec3(0.0f, 0.0f, -1 * zDelta));
        } else if (key == GLFW_KEY_F) {
            *modelMatrix = translate(*modelMatrix, vec3(0.0f, 0.0f, zDelta));
        }
    }
}
