#include <ShadowMap.hpp>
#include <GlErrorCheck.hpp>

#include <MathUtils.hpp>
using MathUtils::degreesToRadians;

#include <glm/glm.hpp>
using glm::transpose;

#include <glm/gtc/matrix_transform.hpp>
using glm::infinitePerspective;

#include <glm/gtx/transform.hpp>
using glm::perspective;
using glm::translate;
using glm::scale;

#include <cstdio>
using std::printf;

int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = ShadowMap::getInstance();
    meshDemo->create(800, 600, "Rendering Multiple Objects");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> ShadowMap::getInstance() {
    static GlfwOpenGlWindow * instance = new ShadowMap();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
ShadowMap::ShadowMap()
        : vao(0), vbo_vertices(0), vbo_normals(0) {
    light.position = vec3(-2.0f, 3.0f, 2.0f);
    light.rgbIntensity = vec3(0.7f, 0.7f, 0.7f);
    light.projectionMatrix = glm::perspective(45.0f, 1.5f, 1.0f, 100.0f);
    light.viewMatrix = glm::lookAt(light.position, vec3(-2.0f, -1.0f, -6.0f),
            vec3(0.0f, 1.0f, 0.0f));
}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void ShadowMap::init()
{
    meshConsolidator =  {"../data/meshes/grid.obj",
                         "../data/meshes/wall.obj",
                         "../data/meshes/wall_back.obj",
                         "../data/meshes/bunny_smooth.obj",
                         "../data/meshes/sphere_smooth.obj" };

    meshConsolidator.getBatchInfo(batchInfoVec);

    setupShaders();
    setupGLBuffers();
    setupMatrices();
    setupShadowFBO();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glDisable(GL_DEPTH_CLAMP);

    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
}

//---------------------------------------------------------------------------------------
void ShadowMap::setupShaders() {
    shaderProgram.loadFromFile("../data/shaders/ADS_withShadow.vert",
                               "../data/shaders/ADS_withShadow.frag");

    shaderProgram.setUniform("ambientIntensity", vec3(0.01f, 0.01f, 0.01f));
    shaderProgram.setUniform("light.rgbIntensity", light.rgbIntensity);

    // Generate VAO and enable vertex attribute arrays for positions and normals.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLint position_Location = shaderProgram.getAttribLocation("vertexPosition");
    glEnableVertexAttribArray(position_Location);
    GLint normal_Location = shaderProgram.getAttribLocation("vertexNormal");
    glEnableVertexAttribArray(normal_Location);

    // Get subroutine indices from fragment shader.
    subroutineIndex_renderDepthValues = glGetSubroutineIndex(shaderProgram.getProgramObject(),
            GL_FRAGMENT_SHADER, "recordDepthValues");

    subroutineIndex_shadeWithShadow = glGetSubroutineIndex(shaderProgram.getProgramObject(),
            GL_FRAGMENT_SHADER, "shadeWithShadow");

    shaderProgram.setUniform("shadowMap", 0); // Use Texture Unit 0.

    checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void ShadowMap::setupGLBuffers()
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
void ShadowMap::setupMatrices() {
    camera.setProjectionMatrix(infinitePerspective(45.0f, (float)windowWidth / windowHeight, 1.0f));
    camera.setPosition(vec3(4.0f, 2.0f, -5.0f));
    camera.lookAt(glm::vec3(0.0f, -1.0f, -10.0f));

    mat4 identity = mat4();
    modelMatrix_grid = translate(identity, vec3(0.0f, -3.8f, -10.0f));
    modelMatrix_bunny = translate(identity, vec3(-2.0f, -3.6f, -12.5f));
    modelMatrix_sphere = translate(identity, vec3(-2.5f, -3.0f, -9.3f));
    modelMatrix_wallLeft = translate(identity, vec3(-5.0f, 0.8f, -12.0f));
    modelMatrix_wallBack = translate(modelMatrix_wallLeft, vec3(5.0f, 0.0f, -5.0f));

    shadowBiasMatrix = mat4( vec4(0.5f,0.0f,0.0f,0.0f),
                             vec4(0.0f,0.5f,0.0f,0.0f),
                             vec4(0.0f,0.0f,0.5f,0.0f),
                             vec4(0.5f,0.5f,0.5f,1.0f) );

    lightSPV = shadowBiasMatrix * light.projectionMatrix * light.viewMatrix;
}

//---------------------------------------------------------------------------------------
void ShadowMap::setupShadowFBO() {
    shadowMapWidth = 1024;
    shadowMapHeight = 1024;

    GLuint depthTexture; // Shadow map texture.
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0,
            GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    GLfloat borderColor[] = {1.0f, 0.0f, 0.0f, 0.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    // Assign shadow map to texture channel 0.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    // Create and setup the FBO.
    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    GLenum drawBuffers[] = {GL_NONE};
    glDrawBuffers(1, drawBuffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    } else {
        printf("Framebuffer is not complete.\n");
    }

    // Revert to the default framebuffer for now.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void ShadowMap::draw()
{
    // Pass 1 (shadow map generation).
    renderDepthPass = true;
    viewMatrix = light.viewMatrix;
    projectionMatrix = light.projectionMatrix;
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, shadowMapWidth, shadowMapHeight);
    shaderProgram.enable();
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutineIndex_renderDepthValues);
    shaderProgram.disable();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    drawScene();
    glFlush();
    glFinish();

    //  Pass 2 (shade scene with shadows)
    renderDepthPass = false;
    viewMatrix = camera.getViewMatrix();
    projectionMatrix = camera.getProjectionMatrix();
    shaderProgram.setUniform("light.position", vec3(viewMatrix * vec4(light.position, 1.0)));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, windowWidth, windowHeight);
    shaderProgram.enable();
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutineIndex_shadeWithShadow);
    shaderProgram.disable();
    glCullFace(GL_BACK);
    drawScene();

    checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void ShadowMap::drawScene() {
    drawGrid();
    drawLeftWall();
    drawBackWall();
    drawBunny();
    drawSphere();
    drawLight();
}

//---------------------------------------------------------------------------------------
void ShadowMap::drawGrid() {
    MaterialProperties m;
    m.emission = vec3(0.0f);
    m.Ka = vec3(1.0f, 1.0f, 1.0f);
    m.Kd = vec3(0.2f, 0.2f, 0.2f);
    m.Ks = 0.3f;
    m.shininess = 10.0f;
    updateMaterialProperties(m);

    modelMatrix = modelMatrix_grid;
    modelMatrix = scale(modelMatrix, vec3(50.0f, 50.0f, 50.0f));
    updateMatrices();

    shaderProgram.enable();
        glDrawArrays(GL_TRIANGLES, batchInfoVec.at(0).startIndex, batchInfoVec.at(0).numIndices);
    shaderProgram.disable();

}

//---------------------------------------------------------------------------------------
void ShadowMap::drawLeftWall() {
    material_shadowBox.emission = vec3(0.0f);
    material_shadowBox.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_shadowBox.Kd = vec3(0.2f, 0.2f, 0.2f);
    material_shadowBox.Ks = 0.3f;
    material_shadowBox.shininess = 10.0f;
    updateMaterialProperties(material_shadowBox);

    modelMatrix = modelMatrix_wallLeft;
    updateMatrices();

    shaderProgram.enable();
        glDrawArrays(GL_TRIANGLES, batchInfoVec.at(1).startIndex, batchInfoVec.at(1).numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void ShadowMap::drawBackWall() {
    material_shadowBox.emission = vec3(0.0f);
    material_shadowBox.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_shadowBox.Kd = vec3(0.2f, 0.2f, 0.2f);
    material_shadowBox.Ks = 0.3f;
    material_shadowBox.shininess = 10.0f;
    updateMaterialProperties(material_shadowBox);

    modelMatrix = modelMatrix_wallBack;
    updateMatrices();

    shaderProgram.enable();
        glDrawArrays(GL_TRIANGLES, batchInfoVec.at(2).startIndex, batchInfoVec.at(2).numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void ShadowMap::drawBunny() {
    material_bunny.emission = vec3(0.0f);
    material_bunny.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_bunny.Kd = vec3(0.1f, 0.3f, 0.8f);
    material_bunny.Ks = 0.2f;
    material_bunny.shininess = 50.0f;
    updateMaterialProperties(material_bunny);

    modelMatrix = modelMatrix_bunny;
    updateMatrices();

    shaderProgram.enable();
        glDrawArrays(GL_TRIANGLES, batchInfoVec.at(3).startIndex, batchInfoVec.at(3).numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void ShadowMap::drawSphere() {
    material_sphere.emission = vec3(0.0f);
    material_sphere.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_sphere.Kd = vec3(0.1f, 0.8f, 0.05f);
    material_sphere.Ks = 0.5f;
    material_sphere.shininess = 50.0f;
    updateMaterialProperties(material_sphere);

    modelMatrix = modelMatrix_sphere;
    updateMatrices();

    shaderProgram.enable();
        glDrawArrays(GL_TRIANGLES, batchInfoVec.at(4).startIndex, batchInfoVec.at(4).numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void ShadowMap::drawLight() {
    material_light.emission = vec3(1.0f, 1.0f, 1.0f);
    material_light.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_light.Kd = vec3(0.1f, 0.3f, 0.8f);
    material_light.Ks = 1.0f;
    material_light.shininess = 1000.0f;
    updateMaterialProperties(material_light);

    // Place cube at light source position and shrink uniformly.
    modelMatrix_light = mat4();
    modelMatrix_light = translate(modelMatrix_light, light.position);
    modelMatrix_light = scale(modelMatrix_light, vec3(0.2f, 0.2f, 0.2f));

    modelMatrix = modelMatrix_light;
    updateMatrices();

    shaderProgram.enable();
        glDrawArrays(GL_TRIANGLES, batchInfoVec.at(4).startIndex, batchInfoVec.at(4).numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void ShadowMap::logic() {
    processKeyInput();
}

//---------------------------------------------------------------------------------------
void ShadowMap::updateMatrices() {
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    shaderProgram.setUniform("ModelViewMatrix", modelViewMatrix);
    shaderProgram.setUniform("NormalMatrix", mat3(modelViewMatrix));
    shaderProgram.setUniform("MVP", projectionMatrix * modelViewMatrix);
    shaderProgram.setUniform("ShadowMatrix", lightSPV * modelMatrix);

}

//---------------------------------------------------------------------------------------
void ShadowMap::updateMaterialProperties(const MaterialProperties & m) {
    // Set uniform material properties
    shaderProgram.setUniform("material.emission", m.emission);
    shaderProgram.setUniform("material.Ka", m.Ka);
    shaderProgram.setUniform("material.Kd", m.Kd);
    shaderProgram.setUniform("material.Ks", m.Ks);
    shaderProgram.setUniform("material.shininess", m.shininess);
}

//---------------------------------------------------------------------------------------
void ShadowMap::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}

//---------------------------------------------------------------------------------------
void ShadowMap::processKeyInput( ) {
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
        camera.lookAt(vec3(-2.0f, -3.6f, -12.5f));
    }
    if (lookAt_sphere) {
        camera.lookAt(vec3(-2.5f, -3.0f, -9.3f));
    }
    if (lookAt_light) {
        camera.lookAt(vec3(-2.0f, 3.0f, 2.0f));
    }
}

//---------------------------------------------------------------------------------------
void ShadowMap::keyInput(int key, int scancode, int action, int mods) {

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
            lookAt_light = false;
        } else if (key == GLFW_KEY_F2) {
            // Look at sphere.
            lookAt_bunny = false;
            lookAt_sphere = true;
            lookAt_light = false;
        } else if (key == GLFW_KEY_F3) {
            // Look at light.
            lookAt_bunny = false;
            lookAt_sphere = false;
            lookAt_light = true;
        } else if (key == GLFW_KEY_SPACE) {
            // Stop looking at target.
            lookAt_bunny = false;
            lookAt_sphere = false;
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
void ShadowMap::mouseScroll(double xOffSet, double yOffSet) {
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

