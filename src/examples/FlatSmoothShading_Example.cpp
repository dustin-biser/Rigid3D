#include <FlatSmoothShading_Example.hpp>
#include <MathUtils.hpp>
#include <GlErrorCheck.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <memory>
#include <ctime>
#include <array>

using namespace std;
using namespace MathUtils;

int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = FlatSmoothShading_Example::getInstance();
    meshDemo->create(800, 600, "Load Multiple Mesh Objects Example");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> FlatSmoothShading_Example::getInstance() {
    static GlfwOpenGlWindow * instance = new FlatSmoothShading_Example();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
FlatSmoothShading_Example::FlatSmoothShading_Example() {
    // Set light and material properties for all objects.
    lightSource.position = vec3(0.0f, 0.0f, 5.0f);
    lightSource.rgbIntensity = vec3(0.9f, 0.9f, 0.9f);

    renderTarget = MeshType::CUBE;
    shadingType = ShadingType::FLAT;
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::setupGLBuffers()
{
    //-- Concatenate vertex data from all meshes.
    size_t totalVertexBytes = cubeMeshFlat.getNumVertexBytes() + sphereMeshFlat.getNumVertexBytes() +
                              torusMeshFlat.getNumVertexBytes() + susanMeshFlat.getNumVertexBytes() +
                              cubeMeshSmooth.getNumVertexBytes() + sphereMeshSmooth.getNumVertexBytes() +
                              torusMeshSmooth.getNumVertexBytes() + susanMeshSmooth.getNumVertexBytes();

    float * vertexDataPtr = (float *)malloc(totalVertexBytes);
    float * data = vertexDataPtr;
    memcpy(data, cubeMeshFlat.getVertexDataPtr(), cubeMeshFlat.getNumVertexBytes());
    data += cubeMeshFlat.getNumVertexBytes() / sizeof(float);
    memcpy(data, sphereMeshFlat.getVertexDataPtr(), sphereMeshFlat.getNumVertexBytes());
    data += sphereMeshFlat.getNumVertexBytes() / sizeof(float);
    memcpy(data, torusMeshFlat.getVertexDataPtr(), torusMeshFlat.getNumVertexBytes());
    data += torusMeshFlat.getNumVertexBytes() / sizeof(float);
    memcpy(data, susanMeshFlat.getVertexDataPtr(), susanMeshFlat.getNumVertexBytes());
    data += susanMeshFlat.getNumVertexBytes() / sizeof(float);
    memcpy(data, cubeMeshSmooth.getVertexDataPtr(), cubeMeshSmooth.getNumVertexBytes());
    data += cubeMeshSmooth.getNumVertexBytes() / sizeof(float);
    memcpy(data, sphereMeshSmooth.getVertexDataPtr(), sphereMeshSmooth.getNumVertexBytes());
    data += sphereMeshSmooth.getNumVertexBytes() / sizeof(float);
    memcpy(data, torusMeshSmooth.getVertexDataPtr(), torusMeshSmooth.getNumVertexBytes());
    data += torusMeshSmooth.getNumVertexBytes() / sizeof(float);
    memcpy(data, susanMeshSmooth.getVertexDataPtr(), susanMeshSmooth.getNumVertexBytes());
    data += susanMeshSmooth.getNumVertexBytes() / sizeof(float);

    //-- Concatenate normal data from all meshes.
    size_t totalNormalBytes = cubeMeshFlat.getNumNormalBytes() + sphereMeshFlat.getNumNormalBytes() +
                              torusMeshFlat.getNumNormalBytes() + susanMeshFlat.getNumNormalBytes() +
                              cubeMeshSmooth.getNumNormalBytes() + sphereMeshFlat.getNumNormalBytes() +
                              torusMeshSmooth.getNumNormalBytes() + susanMeshFlat.getNumNormalBytes();

    float * normalDataPtr = (float *)malloc(totalNormalBytes);
    data = normalDataPtr;
    memcpy(data, cubeMeshFlat.getNormalDataPtr(), cubeMeshFlat.getNumNormalBytes());
    data += cubeMeshFlat.getNumNormalBytes() / sizeof(float);
    memcpy(data, sphereMeshFlat.getNormalDataPtr(), sphereMeshFlat.getNumNormalBytes());
    data += sphereMeshFlat.getNumNormalBytes() / sizeof(float);
    memcpy(data, torusMeshFlat.getNormalDataPtr(), torusMeshFlat.getNumNormalBytes());
    data += torusMeshFlat.getNumNormalBytes() / sizeof(float);
    memcpy(data, susanMeshFlat.getNormalDataPtr(), susanMeshFlat.getNumNormalBytes());
    data += susanMeshFlat.getNumNormalBytes() / sizeof(float);
    memcpy(data, cubeMeshSmooth.getNormalDataPtr(), cubeMeshSmooth.getNumNormalBytes());
    data += cubeMeshSmooth.getNumNormalBytes() / sizeof(float);
    memcpy(data, sphereMeshSmooth.getNormalDataPtr(), sphereMeshSmooth.getNumNormalBytes());
    data += sphereMeshSmooth.getNumNormalBytes() / sizeof(float);
    memcpy(data, torusMeshSmooth.getNormalDataPtr(), torusMeshSmooth.getNumNormalBytes());
    data += torusMeshSmooth.getNumNormalBytes() / sizeof(float);
    memcpy(data, susanMeshSmooth.getNormalDataPtr(), susanMeshSmooth.getNumNormalBytes());
    data += susanMeshSmooth.getNumNormalBytes() / sizeof(float);

    // Register vertex positions with OpenGL
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, totalVertexBytes, vertexDataPtr, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Register normals with OpenGL
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, totalNormalBytes, normalDataPtr, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

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
void FlatSmoothShading_Example::init()
{
    cubeMeshFlat.fromObjFile("../data/meshes/cube.obj");
    sphereMeshFlat.fromObjFile("../data/meshes/sphere.obj");
    torusMeshFlat.fromObjFile("../data/meshes/torus.obj");
    susanMeshFlat.fromObjFile("../data/meshes/susan.obj");
    cubeMeshSmooth.fromObjFile("../data/meshes/cube_smooth.obj");
    sphereMeshSmooth.fromObjFile("../data/meshes/sphere_smooth.obj");
    torusMeshSmooth.fromObjFile("../data/meshes/torus_smooth.obj");
    susanMeshSmooth.fromObjFile("../data/meshes/susan_smooth.obj");

    setupShaders();
    setupGLBuffers();
    setupMatrices();

    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::setupShaders() {
    shaderProgram.loadFromFile("../data/shaders/PerFragLighting.vert",
                               "../data/shaders/PerFragLighting.frag");

    shaderProgram.setUniform("ambientIntensity", vec3(0.1f, 0.1f, 0.1f));
    shaderProgram.setUniform("lightSource.position", lightSource.position);
    shaderProgram.setUniform("lightSource.rgbIntensity", lightSource.rgbIntensity);
    shaderProgram.setUniform("material.Ka", vec3(1.0f, 1.0f, 1.0f));
    shaderProgram.setUniform("material.Kd", vec3(0.1f, 0.1f, 0.8f));
    shaderProgram.setUniform("material.Ks", 0.2f);
    shaderProgram.setUniform("material.shininessFactor", 50.0f);

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
void FlatSmoothShading_Example::setupMatrices() {
    frustum = Frustum(45.0f, 4.0f/3.0f, 1.0f, 100.0f);
    projectionMatrix = frustum.getPerspectiveMatrix();

    viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0, 5.0),
                                      glm::vec3(0.0 , 0.0, -1.0),
                                      glm::vec3(0.0, 1.0, 0.0));

    modelMatrix = glm::mat4(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, 1, 0,
                                   0, 0, 0, 1);

    // Translate the object into view.
    modelMatrix = translate(modelMatrix, 0.0f, 0.0f, -0.3f);

    modelViewMatrix = viewMatrix * modelMatrix;
    normalMatrix = mat3(modelViewMatrix);

    shaderProgram.setUniform("ModelViewMatrix", modelViewMatrix);
    shaderProgram.setUniform("NormalMatrix", normalMatrix);
    shaderProgram.setUniform("ProjectionMatrix", projectionMatrix);
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::draw()
{
    static const GLint cubeFlatStartIndex = 0;
    static const GLint sphereFlatStartIndex = cubeMeshFlat.getNumVertices();
    static const GLint torusFlatStartIndex = sphereFlatStartIndex + sphereMeshFlat.getNumVertices();
    static const GLint susanFlatStartIndex = torusFlatStartIndex + torusMeshFlat.getNumVertices();
    static const GLint cubeSmoothStartIndex = susanFlatStartIndex + susanMeshFlat.getNumVertices();
    static const GLint sphereSmoothStartIndex = cubeSmoothStartIndex + cubeMeshSmooth.getNumVertices();
    static const GLint torusSmoothStartIndex = sphereSmoothStartIndex + sphereMeshSmooth.getNumVertices();
    static const GLint susanSmoothStartIndex = torusSmoothStartIndex + torusMeshSmooth.getNumVertices();

    shaderProgram.enable();
        switch (renderTarget) {
        case (MeshType::CUBE):
            if (shadingType == ShadingType::FLAT) {
                glDrawArrays(GL_TRIANGLES, cubeFlatStartIndex, cubeMeshFlat.getNumVertices());
            } else {
                glDrawArrays(GL_TRIANGLES, cubeSmoothStartIndex, cubeMeshSmooth.getNumVertices());
            }
            break;
        case (MeshType::SPHERE):
            if (shadingType == ShadingType::FLAT) {
                glDrawArrays(GL_TRIANGLES, sphereFlatStartIndex, sphereMeshFlat.getNumVertices());
            } else {
                glDrawArrays(GL_TRIANGLES, sphereSmoothStartIndex, sphereMeshSmooth.getNumVertices());
            }
            break;
        case (MeshType::TORUS):
            if (shadingType == ShadingType::FLAT) {
                glDrawArrays(GL_TRIANGLES, torusFlatStartIndex, torusMeshFlat.getNumVertices());
            } else {
                glDrawArrays(GL_TRIANGLES, torusSmoothStartIndex, torusMeshSmooth.getNumVertices());
            }
            break;
        case (MeshType::SUSAN):
            if (shadingType == ShadingType::FLAT) {
                glDrawArrays(GL_TRIANGLES, susanFlatStartIndex, susanMeshFlat.getNumVertices());
            } else {
                glDrawArrays(GL_TRIANGLES, susanSmoothStartIndex, susanMeshSmooth.getNumVertices());
            }
            break;
        }
    shaderProgram.disable();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::resize(int width, int height)
{
    float aspectRatio = ((float) width) / height;
    float frustumYScale = cotangent(degreesToRadians(frustum.getFieldOfViewY() / 2));

    float frustumXScale = frustumYScale * frustum.getAspectRatio();

    if (width > height) {
        // Shrink the x scale in eye-coordinate space, so that when geometry is
        // projected to ndc-space, it is widened out to become square.
        projectionMatrix[0][0] = frustumXScale / aspectRatio;
        projectionMatrix[1][1] = frustumYScale;
    }
    else {
        // Shrink the y scale in eye-coordinate space, so that when geometry is
        // projected to ndc-space, it is widened out to become square.
        projectionMatrix[0][0] = frustumXScale;
        projectionMatrix[1][1] = frustumYScale * aspectRatio;
    }

    // Use entire window for rendering.
    glViewport(0, 0, width, height);
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::logic() {
    updateMatrices();
    if (pauseLightSource == false) {
        rotateLightSource();
    }
    updateUniformData();
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::updateMatrices() {
    modelViewMatrix = viewMatrix * modelMatrix;
    normalMatrix = mat3(modelViewMatrix);

    shaderProgram.setUniform("ModelViewMatrix", modelViewMatrix);
    shaderProgram.setUniform("NormalMatrix", normalMatrix);
    shaderProgram.setUniform("ProjectionMatrix", projectionMatrix);
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::updateUniformData() {
    shaderProgram.setUniform("lightSource.position", lightSource.position);
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::rotateLightSource() {
    static float t = 0.0f;
    const float period = 15.0f;
    const float omega = 2 * MathUtils::PI / period;
    const float radius = 5.0f;

    t += 0.01f;
    if ( t > period) { t = 0.0f; }

    float x = radius * sin(omega * t);
    float z = radius * cos(omega * t);
    lightSource.position = vec3(x, 0.0f, z);
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::keyInput(int key, int scancode, int action, int mods) {
    GlfwOpenGlWindow::keyInput(key, scancode, action, mods);
    static const float xDelta = 0.5f;
    static const float yDelta = 0.2f;
    static const float zDelta = 0.5f;

    // Select smooth / flat shading
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_F1) {
            shadingType = ShadingType::FLAT;
        } else if (key == GLFW_KEY_F2) {
            shadingType = ShadingType::SMOOTH;
        }
    }

    // Select MeshType to render
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_1:
            renderTarget = MeshType::CUBE;
            break;
        case GLFW_KEY_2:
            renderTarget = MeshType::SPHERE;
            break;
        case GLFW_KEY_3:
            renderTarget = MeshType::TORUS;
            break;
        case GLFW_KEY_4:
            renderTarget = MeshType::SUSAN;
            break;
        default:
            break;
        }

    }

    // Light Source Movement
    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
        if (key == GLFW_KEY_LEFT) {
            lightSource.position -= vec3(xDelta, 0.0f, 0.0f);
            cout << "lightSource.position = " << lightSource.position << endl;
        } else if (key == GLFW_KEY_RIGHT) {
            lightSource.position += vec3(xDelta, 0.0f, 0.0f);
            cout << "lightSource.position = " << lightSource.position << endl;
        } else if (key == GLFW_KEY_UP) {
            lightSource.position += vec3(0.0f, yDelta, 0.0f);
            cout << "lightSource.position = " << lightSource.position << endl;
        } else if (key == GLFW_KEY_DOWN)  {
            lightSource.position -= vec3(0.0f, yDelta, 0.0f);
            cout << "lightSource.position = " << lightSource.position << endl;
        }  else if (key == GLFW_KEY_P) {
            lightSource.position -= vec3(0.0f, 0.0f, zDelta);
            cout << "lightSource.position = " << lightSource.position << endl;
        }  else if (key == GLFW_KEY_L) {
            lightSource.position += vec3(0.0f, 0.0f, zDelta);
            cout << "lightSource.position = " << lightSource.position << endl;
        }
        else if (key == GLFW_KEY_SPACE) {
            pauseLightSource = !pauseLightSource;
            cout << "lightSource.position = " << lightSource.position << endl;
        }
    }

    // Object Movement
    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
        if (key == GLFW_KEY_A) {
            modelMatrix = translate(modelMatrix, -1 * xDelta, 0.0f, 0.0f);
        } else if (key == GLFW_KEY_D) {
            modelMatrix = translate(modelMatrix, xDelta, 0.0f, 0.0f);
        } else if (key == GLFW_KEY_W) {
            modelMatrix = translate(modelMatrix, 0.0f, yDelta, 0.0f);
        } else if (key == GLFW_KEY_S) {
            modelMatrix = translate(modelMatrix, 0.0f, -1 * yDelta, 0.0f);
        } else if (key == GLFW_KEY_R) {
            modelMatrix = translate(modelMatrix, 0.0f, 0.0f, -1 * zDelta);
        } else if (key == GLFW_KEY_F) {
            modelMatrix = translate(modelMatrix, 0.0f, 0.0f, zDelta);
        }
    }
}
