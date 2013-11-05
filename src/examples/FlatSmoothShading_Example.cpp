#include <FlatSmoothShading_Example.hpp>
#include <MathUtils.hpp>
#include <GlErrorCheck.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include <unordered_map>
using std::unordered_map;

using namespace MathUtils;

int main() {
    shared_ptr<GlfwOpenGlWindow> meshDemo = FlatSmoothShading_Example::getInstance();
    meshDemo->create(800, 600, "Flat Versus Smooth Shading Example");

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
FlatSmoothShading_Example::FlatSmoothShading_Example()
        : pauseLightSource(false), vao(0), vbo_vertices(0), vbo_normals(0) {

    // Set light and material properties for all objects.
    lightSource.position = vec3(0.0f, 0.0f, 5.0f);
    lightSource.rgbIntensity = vec3(0.9f, 0.9f, 0.9f);

    renderTarget = MeshType::CUBE;
    shadingType = ShadingType::FLAT;
}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void FlatSmoothShading_Example::init()
{
    meshConsolidator =  {
            {"cube_flat", "../data/meshes/cube.obj"},
            {"sphere_flat", "../data/meshes/sphere.obj"},
            {"torus_flat", "../data/meshes/torus.obj"},
            {"susan_flat", "../data/meshes/susan.obj"},
            {"cube_smooth", "../data/meshes/cube_smooth.obj"},
            {"sphere_smooth", "../data/meshes/sphere_smooth.obj"},
            {"torus_smooth", "../data/meshes/torus_smooth.obj"},
            {"susan_smooth", "../data/meshes/susan_smooth.obj"}
    };

    meshConsolidator.getBatchInfo(batchInfoMap);

    setupShaders();
    setupGLBuffers();
    setupMatrices();

    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::setupGLBuffers()
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
void FlatSmoothShading_Example::setupShaders() {
    shaderProgram.loadFromFile("../data/shaders/PerFragLighting.vert",
                               "../data/shaders/PerFragLighting.frag");

    shaderProgram.setUniform("ambientIntensity", vec3(0.1f, 0.1f, 0.1f));
    shaderProgram.setUniform("lightSource.position", lightSource.position);
    shaderProgram.setUniform("lightSource.rgbIntensity", lightSource.rgbIntensity);
    shaderProgram.setUniform("material.Ka", vec3(1.0f, 1.0f, 1.0f));
    shaderProgram.setUniform("material.Kd", vec3(0.1f, 0.3f, 0.8f));
    shaderProgram.setUniform("material.Ks", 0.3f);
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
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0, 5.0),
                                      glm::vec3(0.0 , 0.0, -1.0),
                                      glm::vec3(0.0, 1.0, 0.0));

    modelMatrix = glm::mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1);

    // Translate the object into view.
    modelMatrix = translate(modelMatrix, vec3(0.0f, 0.0f, -0.3f));

    modelViewMatrix = viewMatrix * modelMatrix;
    normalMatrix = mat3(modelViewMatrix);

    shaderProgram.setUniform("ModelViewMatrix", modelViewMatrix);
    shaderProgram.setUniform("NormalMatrix", normalMatrix);
    shaderProgram.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
}

//---------------------------------------------------------------------------------------
void FlatSmoothShading_Example::draw()
{
    shaderProgram.enable();
        switch (renderTarget) {
        case (MeshType::CUBE):
            if (shadingType == ShadingType::FLAT) {
                glDrawArrays(GL_TRIANGLES, batchInfoMap.at("cube_flat").startIndex, batchInfoMap.at("cube_flat").numIndices);
            } else {
                glDrawArrays(GL_TRIANGLES, batchInfoMap.at("cube_smooth").startIndex, batchInfoMap.at("cube_smooth").numIndices);
            }
            break;
        case (MeshType::SPHERE):
            if (shadingType == ShadingType::FLAT) {
                glDrawArrays(GL_TRIANGLES, batchInfoMap.at("sphere_flat").startIndex, batchInfoMap.at("sphere_flat").numIndices);
            } else {
                glDrawArrays(GL_TRIANGLES, batchInfoMap.at("sphere_smooth").startIndex, batchInfoMap.at("sphere_smooth").numIndices);
            }
            break;
        case (MeshType::TORUS):
            if (shadingType == ShadingType::FLAT) {
                glDrawArrays(GL_TRIANGLES, batchInfoMap.at("torus_flat").startIndex, batchInfoMap.at("torus_flat").numIndices);
            } else {
                glDrawArrays(GL_TRIANGLES, batchInfoMap.at("torus_smooth").startIndex, batchInfoMap.at("torus_smooth").numIndices);
            }
            break;
        case (MeshType::SUSAN):
            if (shadingType == ShadingType::FLAT) {
                glDrawArrays(GL_TRIANGLES, batchInfoMap.at("susan_flat").startIndex, batchInfoMap.at("susan_flat").numIndices);
            } else {
                glDrawArrays(GL_TRIANGLES, batchInfoMap.at("susan_smooth").startIndex, batchInfoMap.at("susan_smooth").numIndices);
            }
            break;
        }
    shaderProgram.disable();

    GlUtils::checkGLErrors(__FILE__, __LINE__);
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
    shaderProgram.setUniform("ProjectionMatrix", camera.getProjectionMatrix());
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
void FlatSmoothShading_Example::keyInput(int key, int action, int mods) {
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
            modelMatrix = translate(modelMatrix, vec3(-1 * xDelta, 0.0f, 0.0f));
        } else if (key == GLFW_KEY_D) {
            modelMatrix = translate(modelMatrix, vec3(xDelta, 0.0f, 0.0f));
        } else if (key == GLFW_KEY_W) {
            modelMatrix = translate(modelMatrix, vec3(0.0f, yDelta, 0.0f));
        } else if (key == GLFW_KEY_S) {
            modelMatrix = translate(modelMatrix, vec3(0.0f, -1 * yDelta, 0.0f));
        } else if (key == GLFW_KEY_R) {
            modelMatrix = translate(modelMatrix, vec3(0.0f, 0.0f, -1 * zDelta));
        } else if (key == GLFW_KEY_F) {
            modelMatrix = translate(modelMatrix, vec3(0.0f, 0.0f, zDelta));
        }
    }
}
