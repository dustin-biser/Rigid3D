#include <LoadMeshObj_Example.hpp>
#include <memory>

using namespace std;
/*
 *  TODO
 *  1. Change glDrawArrays to glDrawElements
 *  2. Setup ARRAY_BUFFER for normals
 *  3. Setup ELEMENTS_ARRAY_BUFFER for indices
 *  4. Setup model-view-perspective matrix uniforms
 *  5. Create shaders with lighting info
 */

int main() {
    shared_ptr<LoadMeshObj_Example> example = make_shared<LoadMeshObj_Example>();
    example->setWindowTitle("Load Mesh Object Example");
    example->start(800, 600);

    return 0;
}


//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::InitializeVertexBuffer()
{
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertexDataBytes(), mesh.getVertices(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//---------------------------------------------------------------------------------------
/*
 * Called after the window and OpenGL are initialized. Called exactly once,
 * before the main loop.
 */
void LoadMeshObj_Example::init()
{

    mesh.fromObjFile("../data/torus.obj");

    InitializeVertexBuffer();

    setupShaders();
    setupMatrices();
    setupVertexBuffer();
    setupVertexArrayObject();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupShaders(){
    shader.loadFromFile("../data/PositionColorNormal.vert", "../data/PhoneLighting.frag");
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupMatrices() {
    frustum = Frustum();
    cameraToClipMatrix = frustum.getPerspectiveMatrix();
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupVertexArrayObject() {
    // TODO - Implement
}


//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    sf::Shader::bind(&shader);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    sf::Shader::bind(NULL);
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

}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::cleanup() {

}

