#include <ShadowMap.hpp>
#include <GlErrorCheck.hpp>

#include <MathUtils.hpp>
using MathUtils::degreesToRadians;

#include <glm/glm.hpp>
using glm::transpose;
using glm::normalize;

#include <glm/gtc/matrix_transform.hpp>
using glm::infinitePerspective;

#include <glm/gtx/transform.hpp>
using glm::perspective;
using glm::translate;
using glm::scale;
using glm::inverse;

#include <unordered_map>
using std::unordered_map;

#include <cstdio>
using std::printf;

#include <cmath>
using std::tan;

int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = ShadowMap::getInstance();
    meshDemo->create(800, 600, "Shadow Map Demo");

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
    spotLight.position = vec3(-2.0f, 6.0f, 2.0f);
    spotLight.rgbIntensity = vec3(1.0f, 1.0f, 1.0f);
    spotLight.frustum = Frustum(45.0f, 1.0f, 1.0f, 50.0f);
    spotLight.center = vec3(0.0f, 0.0f, -15.0f);
    spotLight.viewMatrix = glm::lookAt(spotLight.position, spotLight.center, vec3(0.0f, 1.0f, 0.0f));
    spotLight.exponent = 5.0f;
    spotLight.conicAngle = 90.0f;
}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void ShadowMap::init()
{
    meshConsolidator = {
          {"grid3d", "../data/meshes/grid3d.obj"},
          {"wall", "../data/meshes/wall.obj"},
          {"wall_back", "../data/meshes/wall_back.obj"},
          {"bunny", "../data/meshes/bunny_smooth.obj"},
          {"sphere", "../data/meshes/sphere_smooth.obj"}
    };


    meshConsolidator.getBatchInfo(batchInfoMap);

    setupShaders();
    setupGLBuffers();
    setupMatrices();
    setupShadowFBO();

    glDepthMask(GL_TRUE); // Enable depth buffer for writing to.
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glDisable(GL_DEPTH_CLAMP);

    glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
}

//---------------------------------------------------------------------------------------
void ShadowMap::setupShaders() {
    shaderProgram.loadFromFile("../data/shaders/ADS_withShadow.vert",
                               "../data/shaders/ADS_shadow_spotlight.frag");

    shaderProgram.setUniform("ambientIntensity", vec3(0.01f, 0.01f, 0.01f));
    shaderProgram.setUniform("spotLight.rgbIntensity", spotLight.rgbIntensity);
    shaderProgram.setUniform("spotLight.exponent", spotLight.exponent);
    shaderProgram.setUniform("spotLight.conicAngle", spotLight.conicAngle);

    // Generate VAO and enable vertex attribute arrays for positions and normals.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLint position_Location = shaderProgram.getAttribLocation("vertexPosition");
    glEnableVertexAttribArray(position_Location);
    GLint normal_Location = shaderProgram.getAttribLocation("vertexNormal");
    glEnableVertexAttribArray(normal_Location);

    shaderProgram.setUniform("shadowMap", 0); // Use Texture Unit 0.

    checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void ShadowMap::setupGLBuffers()
{
    // Register vertex positions with OpenGL within the context of the bound VAO.
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexBytes(),
            meshConsolidator.getVertexDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Register normals with OpenGL within the context of the bound VAO.
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumNormalBytes(),
            meshConsolidator.getNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void ShadowMap::setupMatrices() {
    camera.setProjectionMatrix(infinitePerspective(45.0f, (float)windowWidth / windowHeight, 1.0f));
//    camera.setPosition(vec3(4.0f, 2.0f, -5.0f));
    camera.setPosition(vec3(25.0f, 2.0f, 5.0f));
    camera.lookAt(glm::vec3(0.0f, 0.0f, -10.0f));

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
    // Pass 1 (Save shadow map to FBO).
    spotLight.viewMatrix = glm::lookAt(spotLight.position, spotLight.center, vec3(0.0f, 1.0f, 0.0f));
    viewMatrix = spotLight.viewMatrix;
    projectionMatrix = spotLight.frustum.getProjectionMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, shadowMapWidth, shadowMapHeight);
    shaderProgram.setUniformSubroutinesuiv(GL_FRAGMENT_SHADER, "recordDepthValues");
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    drawScene();
    glFlush();
    glFinish();

    //  Pass 2 (Shade scene with shadows)
    viewMatrix = camera.getViewMatrix();
    projectionMatrix = camera.getProjectionMatrix();
    shaderProgram.setUniform("spotLight.position", vec3(viewMatrix * vec4(spotLight.position, 1.0)));
    shaderProgram.setUniform("spotLight.center", vec3(viewMatrix * vec4(spotLight.center, 1.0)));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, windowWidth, windowHeight);
    shaderProgram.setUniformSubroutinesuiv(GL_FRAGMENT_SHADER, "shadeWithShadow");
    glCullFace(GL_BACK);
    drawScene();

    drawLightFrustum();
    checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void ShadowMap::drawScene() {
    glBindVertexArray(vao);
    drawGrid();
    drawLeftWall();
    drawBackWall();
    drawBunny();
    drawSphere();
    drawLight();
    glBindVertexArray(0);
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
    modelMatrix = scale(modelMatrix, vec3(50.0f, 1.1f, 50.0f));
    updateMatrices();

    shaderProgram.enable();
        glDrawArrays(GL_TRIANGLES, batchInfoMap.at("grid3d").startIndex, batchInfoMap.at("grid3d").numIndices);
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
        glDrawArrays(GL_TRIANGLES, batchInfoMap.at("wall").startIndex, batchInfoMap.at("wall").numIndices);
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
    glDrawArrays(GL_TRIANGLES, batchInfoMap.at("wall_back").startIndex,
            batchInfoMap.at("wall_back").numIndices);
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
        glDrawArrays(GL_TRIANGLES, batchInfoMap.at("bunny").startIndex, batchInfoMap.at("bunny").numIndices);
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
        glDrawArrays(GL_TRIANGLES, batchInfoMap.at("sphere").startIndex, batchInfoMap.at("sphere").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void ShadowMap::drawLight() {
    material_light.emission = spotLight.rgbIntensity;
    material_light.Ka = vec3(1.0f, 1.0f, 1.0f);
    material_light.Kd = vec3(0.1f, 0.3f, 0.8f);
    material_light.Ks = 1.0f;
    material_light.shininess = 1000.0f;
    updateMaterialProperties(material_light);

    // Place a sphere at light source position and shrink uniformly.
    modelMatrix_light = translate(mat4(), spotLight.position);
    modelMatrix_light = scale(modelMatrix_light, vec3(0.2f, 0.2f, 0.2f));

    modelMatrix = modelMatrix_light;
    updateMatrices();

    shaderProgram.enable();
        glDrawArrays(GL_TRIANGLES, batchInfoMap.at("sphere").startIndex, batchInfoMap.at("sphere").numIndices);
    shaderProgram.disable();
}

//---------------------------------------------------------------------------------------
void ShadowMap::logic() {
    processKeyInput();
}

//---------------------------------------------------------------------------------------
void ShadowMap::updateMatrices() {
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    mat3 normalMatrix = transpose(inverse(mat3(viewMatrix)));
    lightSPV = shadowBiasMatrix * spotLight.frustum.getProjectionMatrix() * spotLight.viewMatrix;

    shaderProgram.setUniform("ModelViewMatrix", modelViewMatrix);
    shaderProgram.setUniform("NormalMatrix", normalMatrix);
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
        camera.lookAt(spotLight.position);
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

    // Move SpotLight
    vec3 xDelta = vec3(0.2f, 0.0f, 0.0f);
    vec3 yDelta = vec3(0.0f, 0.2f, 0.0f);
    vec3 zDelta = vec3(0.0f, 0.0f, 0.2f);
    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
        if (key == GLFW_KEY_H) {
            spotLight.position -= xDelta;
        } else if (key == GLFW_KEY_K) {
            spotLight.position += xDelta;
        } else if (key == GLFW_KEY_U) {
            spotLight.position += yDelta;
        } else if (key == GLFW_KEY_J) {
            spotLight.position -= yDelta;
        } else if (key == GLFW_KEY_O) {
            spotLight.position -= zDelta;
        } else if (key == GLFW_KEY_L) {
            spotLight.position += zDelta;
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

//---------------------------------------------------------------------------------------
void ShadowMap::reloadShaderProgram() {
    shaderProgram.loadFromFile("../data/shaders/ADS_withShadow.vert",
                               "../data/shaders/ADS_shadow_spotlight.frag");

    shaderProgram.setUniform("ambientIntensity", vec3(0.01f, 0.01f, 0.01f));
    shaderProgram.setUniform("spotLight.rgbIntensity", spotLight.rgbIntensity);
    shaderProgram.setUniform("spotLight.exponent", spotLight.exponent);
    shaderProgram.setUniform("spotLight.conicAngle", spotLight.conicAngle);

    // Generate VAO and enable vertex attribute arrays for positions and normals.
    GLint position_Location = shaderProgram.getAttribLocation("vertexPosition");
    glEnableVertexAttribArray(position_Location);
    GLint normal_Location = shaderProgram.getAttribLocation("vertexNormal");
    glEnableVertexAttribArray(normal_Location);

    shaderProgram.setUniform("shadowMap", 0); // Use Texture Unit 0.

    checkGLErrors(__FILE__, __LINE__);
    cout << "Loading Shader Program" << endl;
}

//---------------------------------------------------------------------------------------
void ShadowMap::drawLightFrustum() {
    float fovy = spotLight.frustum.getFieldOfViewY();
    float zNearDist = spotLight.frustum.getNearZDistance();
    float zFarDist = spotLight.frustum.getFarZDistance();
    float tanAngle = tan(fovy/2);
    float aspect = spotLight.frustum.getAspectRatio();

    float yNear = zNearDist * tanAngle;
    float xNear = aspect * yNear;
    float yFar = zFarDist * tanAngle;
    float xFar = aspect * yFar;
    float zNear = -1.0f * zNearDist;
    float zFar = -1.0f * zFarDist;

    vec3 nearLeftTop(-xNear, yNear, zNear);
    vec3 nearLeftBottom(-xNear, -yNear, zNear);
    vec3 nearRightBottom(xNear, -yNear, zNear);
    vec3 nearRightTop(xNear, yNear, zNear);

    vec3 farLeftTop(-xFar, yFar, zFar);
    vec3 farLeftBottom(-xFar, -yFar, zFar);
    vec3 farRightBottom(xFar, -yFar, zFar);
    vec3 farRightTop(xFar, yFar, zFar);

    static vector<vec3> vertices = {
            nearLeftTop, nearLeftBottom, nearRightBottom, nearRightTop,     // front face
            farLeftTop, farLeftBottom, farRightBottom, farRightTop,         // far face
            nearLeftBottom, nearRightBottom, farRightBottom, farLeftBottom, // bottom face
            nearLeftTop, nearRightTop, farRightTop, farLeftTop,             // top face
            nearLeftBottom, farLeftBottom, farLeftTop, nearLeftTop,         // left face
            nearRightBottom, farRightBottom, farRightTop, nearRightTop      // right face
    };

    mat4 t = translate(mat4(), spotLight.position);
    mat4 rotationMatrix;
    rotationMatrix[0] = spotLight.viewMatrix[0];
    rotationMatrix[1] = spotLight.viewMatrix[1];
    rotationMatrix[2] = spotLight.viewMatrix[2];
    modelMatrix = t * transpose(rotationMatrix);

    static ShaderProgram lightFrustumShader("../data/shaders/LightFrustum.vert",
                                            "../data/shaders/LightFrustum.frag");

    lightFrustumShader.setUniform("vertexColor", vec4(1.0f, 1.0f, 0.0f, 1.0f));
    lightFrustumShader.setUniform("ModelViewMatrix", camera.getViewMatrix() * modelMatrix);
    lightFrustumShader.setUniform("ProjectionMatrix", camera.getProjectionMatrix());

    GLuint vao_frustum;
    glGenVertexArrays(1, &vao_frustum);
    glBindVertexArray(vao_frustum);
    glEnableVertexAttribArray(lightFrustumShader.getAttribLocation("vertexPosition"));

    GLuint vbo_frustum;
    glGenBuffers(1, &vbo_frustum);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_frustum);
    GLsizeiptr numBytes = vertices.size() * 3 * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, numBytes, const_cast<float *>(&((vertices.data())->x)), GL_STATIC_DRAW);
    glVertexAttribPointer(lightFrustumShader.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    checkGLErrors(__FILE__, __LINE__);

    glBindVertexArray(vao_frustum);

    lightFrustumShader.enable();
        int indicesPerFace = 4;
        for(int i = 0; i < 6; i++) {
            glDrawArrays(GL_LINE_LOOP, i*indicesPerFace, indicesPerFace);
        }
    lightFrustumShader.disable();

    glBindVertexArray(0);
    checkGLErrors(__FILE__, __LINE__);
}
