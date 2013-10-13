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
    meshConsolidator =  {"../data/meshes/grid.obj",
                         "../data/meshes/bunny_smooth.obj",
                         "../data/meshes/ship.obj",
                         "../data/meshes/sphere_smooth.obj",
                         "../data/meshes/cube.obj"};

    meshConsolidator.getBatchInfo(batchInfoVec);

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
    modelMatrix_tyrannosaurus = translate(identity, vec3(3.0f, 5.0f, -9.8f));
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
    glDrawArrays(GL_TRIANGLES, batchInfoVec.at(0).startIndex, batchInfoVec.at(0).numIndices);
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
    glDrawArrays(GL_TRIANGLES, batchInfoVec.at(1).startIndex, batchInfoVec.at(1).numIndices);
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
    glDrawArrays(GL_TRIANGLES, batchInfoVec.at(2).startIndex, batchInfoVec.at(2).numIndices);
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
    glDrawArrays(GL_TRIANGLES, batchInfoVec.at(3).startIndex, batchInfoVec.at(3).numIndices);
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
    glDrawArrays(GL_TRIANGLES, batchInfoVec.at(4).startIndex, batchInfoVec.at(4).numIndices);
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
    const float delta = 0.03f;
    const float deltaLarge = 0.1f;

    if (key_r_down) {
        camera.translateRelative(0.0f, 0.0f, delta);
    }
    if (key_f_down) {
        camera.translateRelative(0.0f, 0.0f, -1.0f * delta);
    }
    if (key_q_down) {
        camera.yaw(degreesToRadians(deltaLarge));
    }
    if (key_e_down) {
        camera.yaw(degreesToRadians(-1.0 * deltaLarge));
    }
    if (key_a_down) {
        camera.translateRelative(delta, 0.0f,  0.0f);
    }
    if (key_d_down) {
        camera.translateRelative(-1.0f * delta, 0.0f,  0.0f);
    }
    if (key_w_down) {
        camera.translateRelative(0.0f, delta,  0.0f);
    }
    if (key_s_down) {
        camera.translateRelative(0.0f, -1.0f * delta,  0.0f);
    }
    if (key_left_down) {
        camera.roll(degreesToRadians(-1.0f * deltaLarge));
    }
    if (key_right_down) {
        camera.roll(degreesToRadians(deltaLarge));
    }
    if (key_up_down) {
        camera.pitch(degreesToRadians(delta));
    }
    if (key_down_down) {
        camera.pitch(degreesToRadians(-1.0 * delta));
    }
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
void CameraExample::keyInput(int key, int scancode, int action, int mods) {

    // Handle escape to close window
    GlfwOpenGlWindow::keyInput(key, scancode, action, mods);

    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
        if (key == GLFW_KEY_R) {
            key_r_down = true;
        } else if (key == GLFW_KEY_F) {
            key_f_down = true;
        } else if (key == GLFW_KEY_Q) {
            key_q_down = true;
        } else if (key == GLFW_KEY_E) {
            key_e_down = true;
        } else if (key == GLFW_KEY_W) {
            key_w_down = true;
        } else if (key == GLFW_KEY_S) {
            key_s_down = true;
        } else if (key == GLFW_KEY_A) {
            key_a_down = true;
        } else if (key == GLFW_KEY_D) {
            key_d_down = true;
        } else if (key == GLFW_KEY_LEFT) {
            key_left_down = true;
        } else if (key == GLFW_KEY_RIGHT) {
            key_right_down = true;
        } else if (key == GLFW_KEY_UP) {
            key_up_down = true;
        } else if (key == GLFW_KEY_DOWN) {
            key_down_down = true;
        } else if (key == GLFW_KEY_F1) {
            // Look at bunny.
            lookAt_bunny = true;
            lookAt_sphere = false;
            lookAt_tyrannosaurus = false;
            lookAt_light = false;
        } else if (key == GLFW_KEY_F2) {
            // Look at sphere.
            lookAt_bunny = false;
            lookAt_sphere = true;
            lookAt_tyrannosaurus = false;
            lookAt_light = false;
        } else if (key == GLFW_KEY_F3) {
            // Look at tyrannosaurus.
            lookAt_bunny = false;
            lookAt_sphere = false;
            lookAt_tyrannosaurus = true;
            lookAt_light = false;
        } else if (key == GLFW_KEY_F4) {
            // Look at tyrannosaurus.
            lookAt_bunny = false;
            lookAt_sphere = false;
            lookAt_tyrannosaurus = false;
            lookAt_light = true;
        } else if (key == GLFW_KEY_SPACE) {
            lookAt_bunny = false;
            lookAt_sphere = false;
            lookAt_tyrannosaurus = false;
            lookAt_light = false;
        }
    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_R) {
            key_r_down = false;
        } else if (key == GLFW_KEY_F) {
            key_f_down = false;
        } else if (key == GLFW_KEY_Q) {
            key_q_down = false;
        } else if (key == GLFW_KEY_E) {
            key_e_down = false;
        } else if (key == GLFW_KEY_W) {
            key_w_down = false;
        } else if (key == GLFW_KEY_S) {
            key_s_down = false;
        } else if (key == GLFW_KEY_A) {
            key_a_down = false;
        } else if (key == GLFW_KEY_D) {
            key_d_down = false;
        } else if (key == GLFW_KEY_LEFT) {
            key_left_down = false;
        } else if (key == GLFW_KEY_RIGHT) {
            key_right_down = false;
        } else if (key == GLFW_KEY_UP) {
            key_up_down = false;
        } else if (key == GLFW_KEY_DOWN) {
            key_down_down = false;
        }
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

    cout << "fieldOfViewY: " << fieldOfViewY << endl;
    camera.setFieldOfViewY(fieldOfViewY);
}
