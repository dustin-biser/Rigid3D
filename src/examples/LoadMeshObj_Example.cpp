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
void LoadMeshObj_Example::setupGlData()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Register vertex data with OpenGL
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertexDataBytes(), mesh.getVertexDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(position_AttribLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // TODO - Normals must be transformed differently than vertices
    // when moving the camera.
    // See: http://www.songho.ca/opengl/gl_normaltransform.html

    // Register normals with OpenGL
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, mesh.getNormalDataBytes(), mesh.getNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(normal_AttribLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

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
    setupGlData();
    setupMatrices();
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupShaders() {
    shaderProgram.loadFromFile("data/PositionColorNormal.vert", "data/PhoneLighting.frag");

    this->position_AttribLocation = shaderProgram.getAttribLocation("position");
    this->color_AttribLocation = shaderProgram.getAttribLocation("inDiffuseColor");
    this->normal_AttribLocation = shaderProgram.getAttribLocation("normal");
}

//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::setupMatrices() {
    frustum = Frustum();
    cameraToClipMatrix = frustum.getPerspectiveMatrix();
}


//---------------------------------------------------------------------------------------
void LoadMeshObj_Example::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
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

