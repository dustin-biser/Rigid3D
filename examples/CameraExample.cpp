// CameraExample.cpp

#include "CameraExample.hpp"
#include "GlErrorCheck.hpp"

#include "Trigonometry.hpp"
using Rigid3D::degreesToRadians;

#include <glm/gtx/transform.hpp>
using glm::translate;
using glm::scale;

int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = CameraExample::getInstance();
    meshDemo->create(800, 600, "Rendering Multiple Objects");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> CameraExample::getInstance() {
    static GlfwOpenGlWindow * instance = new CameraExample();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
CameraExample::CameraExample()
        : vao(0), vbo_vertices(0), vbo_normals(0) {
}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void CameraExample::init()
{
    meshConsolidator =  {
            {"grid", "../data/meshes/grid.obj"},
            {"bunny", "../data/meshes/bunny_smooth.obj"},
            {"tyrannosaurus", "../data/meshes/tyrannosaurus_smooth.obj"},
            {"sphere", "../data/meshes/sphere_smooth.obj"},
            {"cube", "../data/meshes/cube.obj"}
    };

    meshConsolidator.getBatchInfo(batchInfoMap);

    glGenVertexArrays(1, &vao);

    lightSource.position = vec3(-2.0f, 3.0f, 2.0f);
    lightSource.rgbIntensity = vec3(0.9f, 0.9f, 0.9f);

    setupShaders();
    setupRenderables();
    setupGLBuffers();

    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
}

//---------------------------------------------------------------------------------------
void CameraExample::setupShaders() {
    shaderProgram.loadFromFile("../data/shaders/Pos-Norm-Tex-Color.vert",
                               "../data/shaders/PerFragLighting_withWorldLight.frag");

    shaderProgram.setUniform("ambientIntensity", vec3(0.1f, 0.1f, 0.1f));
    shaderProgram.setUniform("lightSource.position", lightSource.position);
    shaderProgram.setUniform("lightSource.rgbIntensity", lightSource.rgbIntensity);

    glBindVertexArray(vao);

    GLint position_Location = shaderProgram.getAttribLocation("vertexPosition");
    glEnableVertexAttribArray(position_Location);
    GLint normal_Location = shaderProgram.getAttribLocation("vertexNormal");
    glEnableVertexAttribArray(normal_Location);

    glBindVertexArray(0);

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void CameraExample::setupRenderables() {
    grid          = Renderable(&vao, &shaderProgram, &(batchInfoMap.at("grid")));
    bunny         = Renderable(&vao, &shaderProgram, &(batchInfoMap.at("bunny")));
    tyrannosaurus = Renderable(&vao, &shaderProgram, &(batchInfoMap.at("tyrannosaurus")));
    sphere        = Renderable(&vao, &shaderProgram, &(batchInfoMap.at("sphere")));
    light         = Renderable(&vao, &shaderProgram, &(batchInfoMap.at("cube")));

    renderContext.viewMatrix = camera.getViewMatrix();
    renderContext.projectionMatrix = camera.getProjectionMatrix();

    grid.setPosition(vec3(0.0f, -3.8f, -10.0f));
    grid.setScale(vec3(20.0f, 1.0f, 20.0f));
    grid.setEmissionLevels(vec3(0.0f));
    grid.setAmbientLevels(vec3(1.0f, 1.0f, 1.0f));
    grid.setDiffuseLevels(vec3(0.2f, 0.2f, 0.2f));
    grid.setSpecularIntensity(0.3f);
    grid.setShininessFactor(10.0f);

    bunny.setPosition(vec3(-3.0f, -3.6f, -11.5f));
    bunny.setEmissionLevels(vec3(0.0f));
    bunny.setAmbientLevels(vec3(1.0f, 1.0f, 1.0f));
    bunny.setDiffuseLevels(vec3(0.1f, 0.3f, 0.8f));
    bunny.setSpecularIntensity(0.2f);
    bunny.setShininessFactor(50.0f);

    tyrannosaurus.setPosition(vec3(3.0f, -1.5f, -9.8f));
    tyrannosaurus.setScale(vec3(1.7f));
    tyrannosaurus.setEmissionLevels(vec3(0.0f));
    tyrannosaurus.setAmbientLevels(vec3(1.0f, 1.0f, 1.0f));
    tyrannosaurus.setDiffuseLevels(vec3(0.6f, 0.1f, 0.05f));
    tyrannosaurus.setSpecularIntensity(0.5f);
    tyrannosaurus.setShininessFactor(100.0f);

    sphere.setPosition(vec3(-1.5f, -3.0f, -6.5f));
    sphere.setEmissionLevels(vec3(0.0f));
    sphere.setAmbientLevels(vec3(1.0f, 1.0f, 1.0f));
    sphere.setDiffuseLevels(vec3(0.1f, 0.8f, 0.05f));
    sphere.setSpecularIntensity(0.5f);
    sphere.setShininessFactor(100.0f);

    light.setPosition(lightSource.position);
    light.setScale(vec3(0.2f));
    light.setEmissionLevels(vec3(1.0f));
    light.setAmbientLevels(vec3(1.0f));
    light.setDiffuseLevels(vec3(0.1f, 0.3f, 0.8f));
    light.setSpecularIntensity(1.0f);
    light.setShininessFactor(100.0f);
}

//---------------------------------------------------------------------------------------
void CameraExample::setupGLBuffers()
{
    glBindVertexArray(vao);
    // Copy position data to OpenGL buffer.
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(), meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy normal data to OpenGL buffer.
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(), meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void CameraExample::draw()
{
    grid.render(renderContext);
    bunny.render(renderContext);
    tyrannosaurus.render(renderContext);
    sphere.render(renderContext);
    light.render(renderContext);
}

//---------------------------------------------------------------------------------------
void CameraExample::logic() {
    renderContext.viewMatrix = camera.getViewMatrix();
    renderContext.projectionMatrix = camera.getProjectionMatrix();

    // Transform lightSoruce position to eye space.
    vec4 eyePos = camera.getViewMatrix() * vec4(lightSource.position, 1.0f);
    shaderProgram.setUniform("lightSource.position", vec3(eyePos));
}


//---------------------------------------------------------------------------------------
void CameraExample::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}
