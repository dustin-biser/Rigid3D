#include <LoadMeshObj_Example.hpp>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

// TODO - Normals must be transformed differently than vertices
// when moving the camera.
// See: http://www.songho.ca/opengl/gl_normaltransform.html

int main() {
    shared_ptr<LoadMeshObj_Example> example = make_shared<LoadMeshObj_Example>();
    example->setWindowTitle("Load Mesh Object Example");
    example->start(800, 600);

    return 0;
}


//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupGLBuffers()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Register vertex data with OpenGL
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertexDataBytes(), mesh.getVertexDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(position_AttribLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // Register normals with OpenGL
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, mesh.getNormalDataBytes(), mesh.getNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(normal_AttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Register index elements with OpenGL
    glGenBuffers(1, &vbo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexDataBytes(), mesh.getIndexDataPtr(), GL_STATIC_DRAW);
}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void LoadMeshObj_Example::init()
{
    mesh.fromObjFile("data/cube.obj");

    setupShaders();
    setupGLBuffers();
    setupMatrices();
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupShaders() {
    shaderProgram.loadFromFile("data/UniformColor.vert", "data/VertexColors.frag");

    // Acquire vertex attribute locations.
    this->position_AttribLocation = shaderProgram.getAttribLocation("position");
    this->color_Location = shaderProgram.getUniformLocation("color");
    this->worldToCamera_Location = shaderProgram.getUniformLocation("worldToCameraMatrix");
    this->cameraToClip_Location = shaderProgram.getUniformLocation("cameraToClipMatrix");
    this->modelToWorld_Location = shaderProgram.getUniformLocation("modelToWorldMatrix");

    // Enable vertex attribute arrays so we can send data to vertex shader.
    glEnableVertexAttribArray(position_AttribLocation);

    // Setup uniform color data
    glUniform4f(color_Location, 0.6f, 0.2f, 0.2f, 1.0f);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupMatrices() {
    frustum = Frustum();
    cameraToClipMatrix = frustum.getPerspectiveMatrix();

    worldToCameraMatrix = glm::lookAt(glm::vec3(0,0, 10), glm::vec3(0,0,0), glm::vec3(0,1,0));

    modelToWorldMatrix = glm::mat4(1, 0, 0, 0,
                                   0, 1, 0, 0,
                                   0, 0, 1, 0,
                                   0, 0, 0, 1);

    // Register uniform matrix data to vertex shader
    glUniformMatrix4fv(worldToCamera_Location, 1, GL_FALSE, glm::value_ptr(worldToCameraMatrix));
    glUniformMatrix4fv(cameraToClip_Location, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
    glUniformMatrix4fv(modelToWorld_Location, 1, GL_FALSE, glm::value_ptr(modelToWorldMatrix));
}


//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.begin();
    glDrawElements(GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_BYTE, 0);
    shaderProgram.end();

}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::resize (int width, int height)
{
    float aspectRatio = ((float) width) / height;
    float frustumYScale = MathUtils::coTangent(MathUtils::degreesToRadians(frustum.fieldOfViewY / 2));
    float frustumXScale = frustumYScale * frustum.aspectRatio;

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
    // Pass updated matrix data to vertex shader
    glUniformMatrix4fv(cameraToClip_Location, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::cleanup() {

}

