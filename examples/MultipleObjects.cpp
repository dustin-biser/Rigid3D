#include <MultipleObjects.hpp>
#include <GlErrorCheck.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Trigonometry.hpp"
using Rigid3D::PI;

int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = MultipleObjects::getInstance();
    meshDemo->create(800, 600, "Rendering Multiple Objects");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> MultipleObjects::getInstance() {
    static GlfwOpenGlWindow * instance = new MultipleObjects();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
MultipleObjects::MultipleObjects()
        : pauseLightSource(false), vao(0), vbo_vertices(0), vbo_normals(0) {

    lightSource.rgbIntensity = vec3(0.9f, 0.9f, 0.9f);
}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void MultipleObjects::init()
{
    meshConsolidator = {
            {"grid", "../data/meshes/grid.obj"},
            {"bunny_smooth", "../data/meshes/bunny_smooth.obj"},
            {"tyrannosaurus_smooth", "../data/meshes/tyrannosaurus_smooth.obj"},
            {"sphere_smooth", "../data/meshes/sphere_smooth.obj"},
            {"cube", "../data/meshes/cube.obj"}
    };

    meshConsolidator.getBatchInfo(batchInfoMap);

    setupShaders();
    setupGLBuffers();
    setupMatrices();

    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
}

//---------------------------------------------------------------------------------------
void MultipleObjects::setupShaders() {
    shaderProgram.loadFromFile("../data/shaders/PerFragLighting_withWorldLightPos.vert",
                               "../data/shaders/PerFragLighting_withWorldLightPos.frag");

    shaderProgram.setUniform("ambientIntensity", vec3(0.1f, 0.1f, 0.1f));
    shaderProgram.setUniform("lightSource.rgbIntensity", lightSource.rgbIntensity);

    // Generate VAO and enable vertex attribute arrays for positions and normals.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLint position_Location = shaderProgram.getAttribLocation("vertexPosition");
    glEnableVertexAttribArray(position_Location);
    GLint normal_Location = shaderProgram.getAttribLocation("vertexNormal");
    glEnableVertexAttribArray(normal_Location);

    Rigid3D::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void MultipleObjects::setupGLBuffers()
{
    // Register vertex positions with OpenGL within the context of the bound VAO.
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(), meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Register normals with OpenGL within the context of the bound VAO.
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(), meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Rigid3D::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void MultipleObjects::setupMatrices() {
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 1.0, 5.0),
                             glm::vec3(0.0 , 0.0, -6.0),
                             glm::vec3(0.0, 1.0, 0.0));

    mat4 identity = mat4();

    normalMatrix = mat3(viewMatrix * identity);

    modelMatrix_grid = translate(identity, vec3(0.0f, -3.8f, -10.0f));
    modelMatrix_bunny = translate(identity, vec3(-3.0f, -3.6f, -11.5f));
    modelMatrix_tyrannosaurus = translate(identity, vec3(3.0f, -2.4f, -9.8f));
    modelMatrix_sphere = translate(identity, vec3(-0.5f, -3.0f, -6.5f));

    shaderProgram.setUniform("ViewMatrix", viewMatrix);
    shaderProgram.setUniform("NormalMatrix", normalMatrix);
    shaderProgram.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
}

//---------------------------------------------------------------------------------------
void MultipleObjects::draw()
{
    drawGrid();
    drawBunny();
    drawTyrannosaurus();
    drawSphere();
    drawLight();

    Rigid3D::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void MultipleObjects::drawGrid() {
    material_grid.emission = vec3(0.0f);
    material_grid.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_grid.Kd = vec3(0.2f, 0.2f, 0.2f);
    material_grid.Ks = 0.3f;
    material_grid.shininessFactor = 10.0f;

    // Set uniform material properties
    shaderProgram.setUniform("material.emission", material_grid.emission);
    shaderProgram.setUniform("material.Ka", material_grid.Ka);
    shaderProgram.setUniform("material.Kd", material_grid.Kd);
    shaderProgram.setUniform("material.Ks", material_grid.Ks);
    shaderProgram.setUniform("material.shininessFactor", material_grid.shininessFactor);

    shaderProgram.setUniform("ModelMatrix", modelMatrix_grid);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("grid").startIndex, batchInfoMap.at("grid").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void MultipleObjects::drawBunny() {
    material_bunny.emission = vec3(0.0f);
    material_bunny.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_bunny.Kd = vec3(0.1f, 0.3f, 0.8f);
    material_bunny.Ks = 0.3f;
    material_bunny.shininessFactor = 50.0f;

    // Set uniform material properties
    shaderProgram.setUniform("material.emission", material_bunny.emission);
    shaderProgram.setUniform("material.Ka", material_bunny.Ka);
    shaderProgram.setUniform("material.Kd", material_bunny.Kd);
    shaderProgram.setUniform("material.Ks", material_bunny.Ks);
    shaderProgram.setUniform("material.shininessFactor", material_bunny.shininessFactor);

    shaderProgram.setUniform("ModelMatrix", modelMatrix_bunny);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("bunny_smooth").startIndex, batchInfoMap.at("bunny_smooth").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void MultipleObjects::drawTyrannosaurus() {
    material_tyrannosaurus.emission = vec3(0.0f);
    material_tyrannosaurus.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_tyrannosaurus.Kd = vec3(0.6f, 0.1f, 0.05f);
    material_tyrannosaurus.Ks = 0.3f;
    material_tyrannosaurus.shininessFactor = 50.0f;

    // Set uniform material properties
    shaderProgram.setUniform("material.emission", material_tyrannosaurus.emission);
    shaderProgram.setUniform("material.Ka", material_tyrannosaurus.Ka);
    shaderProgram.setUniform("material.Kd", material_tyrannosaurus.Kd);
    shaderProgram.setUniform("material.Ks", material_tyrannosaurus.Ks);
    shaderProgram.setUniform("material.shininessFactor", material_tyrannosaurus.shininessFactor);

    shaderProgram.setUniform("ModelMatrix", modelMatrix_tyrannosaurus);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("tyrannosaurus_smooth").startIndex, batchInfoMap.at("tyrannosaurus_smooth").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void MultipleObjects::drawSphere() {
    material_sphere.emission = vec3(0.0f);
    material_sphere.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_sphere.Kd = vec3(0.1f, 0.8f, 0.05f);
    material_sphere.Ks = 0.3f;
    material_sphere.shininessFactor = 50.0f;

    // Set uniform material properties
    shaderProgram.setUniform("material.emission", material_sphere.emission);
    shaderProgram.setUniform("material.Ka", material_sphere.Ka);
    shaderProgram.setUniform("material.Kd", material_sphere.Kd);
    shaderProgram.setUniform("material.Ks", material_sphere.Ks);
    shaderProgram.setUniform("material.shininessFactor", material_sphere.shininessFactor);

    shaderProgram.setUniform("ModelMatrix", modelMatrix_sphere);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("sphere_smooth").startIndex, batchInfoMap.at("sphere_smooth").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void MultipleObjects::drawLight() {
    material_light.emission = vec3(1.0f, 1.0f, 1.0f);
    material_light.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_light.Kd = vec3(0.1f, 0.3f, 0.8f);
    material_light.Ks = 0.3f;
    material_light.shininessFactor = 50.0f;

    // Set uniform material properties
    shaderProgram.setUniform("material.emission", material_light.emission);
    shaderProgram.setUniform("material.Ka", material_light.Ka);
    shaderProgram.setUniform("material.Kd", material_light.Kd);
    shaderProgram.setUniform("material.Ks", material_light.Ks);
    shaderProgram.setUniform("material.shininessFactor", material_light.shininessFactor);

    // Place cube at light source position and shrink uniformly.
    modelMatrix_light = mat4();
    modelMatrix_light = translate(modelMatrix_light, lightSource.position);
    modelMatrix_light = scale(modelMatrix_light, vec3(0.2f, 0.2f, 0.2f));

    shaderProgram.setUniform("ModelMatrix", modelMatrix_light);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("cube").startIndex, batchInfoMap.at("cube").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void MultipleObjects::logic() {
    updateMatrices();
    if (pauseLightSource == false) {
        rotateLightSource();
    }
    updateUniformData();
}

//---------------------------------------------------------------------------------------
void MultipleObjects::updateMatrices() {
    shaderProgram.setUniform("NormalMatrix", normalMatrix);
    shaderProgram.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
}

//---------------------------------------------------------------------------------------
void MultipleObjects::updateUniformData() {
    shaderProgram.setUniform("lightSource.position", lightSource.position);
}

//---------------------------------------------------------------------------------------
void MultipleObjects::rotateLightSource() {
    static float t = 0.0f;
    const float period = 15.0f;
    const float omega = 2 * PI / period;
    const float radius = 10.0f;
    const vec3 center = vec3(0.0f, 2.0f, -10.0f);

    t += 0.01f;
    if ( t > period) { t = 0.0f; }

    float x = radius * sin(omega * t);
    float z = radius * cos(omega * t);
    lightSource.position = center + vec3(x, 0.0f, z);
}

//---------------------------------------------------------------------------------------
void MultipleObjects::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}

//---------------------------------------------------------------------------------------
void MultipleObjects::keyInput(int key, int action, int mods) {
    static const float xDelta = 0.5f;
    static const float yDelta = 0.2f;
    static const float zDelta = 0.5f;
    static mat4 *modelMatrix = &this->modelMatrix_grid;

    // Light Source Movement
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE) {
            pauseLightSource = !pauseLightSource;
        }
    }

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
            modelMatrix = &this->modelMatrix_tyrannosaurus;
            break;
        case GLFW_KEY_F4:
            modelMatrix = &this->modelMatrix_sphere;
            break;
        default:
            break;
        }
    }

    // Move light source
    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
        if (key == GLFW_KEY_UP) {
            lightSource.position -= vec3(0.0f, 0.0f, zDelta);
        }
        else if (key == GLFW_KEY_DOWN) {
            lightSource.position += vec3(0.0f, 0.0f, zDelta);
        }
        else if (key == GLFW_KEY_LEFT) {
            lightSource.position -= vec3(xDelta, 0.0f, 0.0f);
        }
        else if (key == GLFW_KEY_RIGHT) {
            lightSource.position += vec3(xDelta, 0.0f, 0.0f);
        }
        else if (key == GLFW_KEY_PAGE_UP) {
            lightSource.position += vec3(0.0f, yDelta, 0.0f);
        }
        else if (key == GLFW_KEY_PAGE_DOWN) {
            lightSource.position -= vec3(0.0f, yDelta, 0.0f);
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
