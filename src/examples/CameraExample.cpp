#include <CameraExample.hpp>
#include <GlErrorCheck.hpp>

#include <MathUtils.hpp>
using MathUtils::degreesToRadians;

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

    lightSource.position = vec3(-2.0f, 3.0f, 2.0f);
    lightSource.rgbIntensity = vec3(0.9f, 0.9f, 0.9f);
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
void CameraExample::setupShaders() {
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

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void CameraExample::setupGLBuffers()
{
    // Register vertex positions with OpenGL within the context of the bound VAO.
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexBytes(), meshConsolidator.getVertexDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Register normals with OpenGL within the context of the bound VAO.
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumNormalBytes(), meshConsolidator.getNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void CameraExample::setupMatrices() {
    camera.lookAt(glm::vec3(0.0f, 1.0, 5.0),
                  glm::vec3(0.0f, -1.0f, -5.0f),
                  glm::vec3(0.0, 1.0, 0.0));

    mat4 identity = mat4();

    normalMatrix = mat3(camera.getViewMatrix());

    modelMatrix_grid = translate(identity, vec3(0.0f, -3.8f, -10.0f));
    modelMatrix_bunny = translate(identity, vec3(-3.0f, -3.6f, -11.5f));
    modelMatrix_tyrannosaurus = translate(identity, vec3(3.0f, -1.5f, -9.8f));
    modelMatrix_tyrannosaurus = scale(modelMatrix_tyrannosaurus, vec3(1.7f));
    modelMatrix_sphere = translate(identity, vec3(-1.5f, -3.0f, -6.5f));

    shaderProgram.setUniform("ViewMatrix", camera.getViewMatrix());
    shaderProgram.setUniform("NormalMatrix", normalMatrix);
    shaderProgram.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
}

//---------------------------------------------------------------------------------------
void CameraExample::draw()
{
    drawGrid();
    drawBunny();
    drawTyrannosaurus();
    drawSphere();
    drawLight();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void CameraExample::drawGrid() {
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

    // Stretch grid to make larger.
    modelMatrix_grid[0][0] = 50.0f;
    modelMatrix_grid[1][1] = 50.0f;
    modelMatrix_grid[2][2] = 50.0f;
    shaderProgram.setUniform("ModelMatrix", modelMatrix_grid);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("grid").startIndex, batchInfoMap.at("grid").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void CameraExample::drawBunny() {
    material_bunny.emission = vec3(0.0f);
    material_bunny.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_bunny.Kd = vec3(0.1f, 0.3f, 0.8f);
    material_bunny.Ks = 0.2f;
    material_bunny.shininessFactor = 50.0f;

    // Set uniform material properties
    shaderProgram.setUniform("material.emission", material_bunny.emission);
    shaderProgram.setUniform("material.Ka", material_bunny.Ka);
    shaderProgram.setUniform("material.Kd", material_bunny.Kd);
    shaderProgram.setUniform("material.Ks", material_bunny.Ks);
    shaderProgram.setUniform("material.shininessFactor", material_bunny.shininessFactor);

    shaderProgram.setUniform("ModelMatrix", modelMatrix_bunny);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("bunny_smooth").startIndex,
            batchInfoMap.at("bunny_smooth").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void CameraExample::drawTyrannosaurus() {
    material_tyrannosaurus.emission = vec3(0.0f);
    material_tyrannosaurus.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_tyrannosaurus.Kd = vec3(0.6f, 0.1f, 0.05f);
    material_tyrannosaurus.Ks = 0.5f;
    material_tyrannosaurus.shininessFactor = 100.0f;

    // Set uniform material properties
    shaderProgram.setUniform("material.emission", material_tyrannosaurus.emission);
    shaderProgram.setUniform("material.Ka", material_tyrannosaurus.Ka);
    shaderProgram.setUniform("material.Kd", material_tyrannosaurus.Kd);
    shaderProgram.setUniform("material.Ks", material_tyrannosaurus.Ks);
    shaderProgram.setUniform("material.shininessFactor", material_tyrannosaurus.shininessFactor);

    shaderProgram.setUniform("ModelMatrix", modelMatrix_tyrannosaurus);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("tyrannosaurus_smooth").startIndex,
            batchInfoMap.at("tyrannosaurus_smooth").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void CameraExample::drawSphere() {
    material_sphere.emission = vec3(0.0f);
    material_sphere.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_sphere.Kd = vec3(0.1f, 0.8f, 0.05f);
    material_sphere.Ks = 0.5f;
    material_sphere.shininessFactor = 50.0f;

    // Set uniform material properties
    shaderProgram.setUniform("material.emission", material_sphere.emission);
    shaderProgram.setUniform("material.Ka", material_sphere.Ka);
    shaderProgram.setUniform("material.Kd", material_sphere.Kd);
    shaderProgram.setUniform("material.Ks", material_sphere.Ks);
    shaderProgram.setUniform("material.shininessFactor", material_sphere.shininessFactor);

    shaderProgram.setUniform("ModelMatrix", modelMatrix_sphere);
    shaderProgram.enable();
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("sphere_smooth").startIndex,
            batchInfoMap.at("sphere_smooth").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void CameraExample::drawLight() {
    material_light.emission = vec3(1.0f, 1.0f, 1.0f);
    material_light.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_light.Kd = vec3(0.1f, 0.3f, 0.8f);
    material_light.Ks = 1.0f;
    material_light.shininessFactor = 1000.0f;

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
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("cube").startIndex,
            batchInfoMap.at("cube").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void CameraExample::logic() {
    updateUniformData();
    processKeyInput();
}

//---------------------------------------------------------------------------------------
void CameraExample::updateUniformData() {
    shaderProgram.setUniform("lightSource.position", lightSource.position);
    updateMatrices();
}

//---------------------------------------------------------------------------------------
void CameraExample::updateMatrices() {
    normalMatrix = mat3(camera.getViewMatrix());

    shaderProgram.setUniform("NormalMatrix", normalMatrix);
    shaderProgram.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
    shaderProgram.setUniform("ViewMatrix", camera.getViewMatrix());
}

//---------------------------------------------------------------------------------------
void CameraExample::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}

//---------------------------------------------------------------------------------------
void CameraExample::processKeyInput( ) {
    if (lookAt_bunny) {
        camera.lookAt(vec3(-3.0f, -3.6f, -11.5f));
    }
    if (lookAt_sphere) {
        camera.lookAt(vec3(-1.5f, -3.0f, -6.5f));
    }
    if (lookAt_tyrannosaurus) {
        camera.lookAt(vec3(3.0f, -2.4f, -9.8f));
    }
    if (lookAt_light) {
        camera.lookAt(vec3(-2.0f, 3.0f, 2.0f));
    }
}


//---------------------------------------------------------------------------------------
void CameraExample::mouseScroll(double xOffSet, double yOffSet) {
    static float fieldOfViewY = 45.0f;
    static const float delta = 5.0f;

    if (yOffSet < 0) {
       fieldOfViewY += delta;
    } else if (yOffSet > 0) {
       fieldOfViewY -= delta;
    }

    // Camp fieldOfViewY to [0, 180]
    if (fieldOfViewY < 0.0f) {
        fieldOfViewY = 0.0f;
    } else if (fieldOfViewY > 180.0f) {
        fieldOfViewY = 180.0f;
    }

    std::cout << "fieldOfViewY: " << fieldOfViewY << std::endl;
    camera.setFieldOfViewY(fieldOfViewY);
}
