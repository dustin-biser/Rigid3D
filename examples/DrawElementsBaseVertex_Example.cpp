#include "DrawElementsBaseVertex_Example.hpp"

int main() {
    shared_ptr<GlfwOpenGlWindow> demo = DrawElementsBaseVertex_Example::getInstance();
    demo->create(800, 600, "DrawElementsBaseVertex Example");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> DrawElementsBaseVertex_Example::getInstance() {
    static GlfwOpenGlWindow * instance = new DrawElementsBaseVertex_Example();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
DrawElementsBaseVertex_Example::DrawElementsBaseVertex_Example()
    : vao(0), vbo(0) {
}

//---------------------------------------------------------------------------------------
void DrawElementsBaseVertex_Example::init()
{
//    meshConsolidator =  {
//        {"grid", "data/meshes/grid.obj"},
//        {"bunny", "data/meshes/bunny_smooth.obj"},
//        {"tyrannosaurus", "data/meshes/tyrannosaurus_smooth.obj"},
//        {"sphere", "data/meshes/sphere_smooth.obj"},
//        {"cube", "data/meshes/cube.obj"}
//    };
//
//    meshConsolidator.getBatchInfo(batchInfoMap);

    glGenVertexArrays(1, &vao);


    setupShaders();
    setupGLBuffers();

    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
}

//---------------------------------------------------------------------------------------
void DrawElementsBaseVertex_Example::setupShaders() {
//    shaderProgram.loadFromFile("data/shaders/Pos-Norm-Tex-Color.vert",
//                               "data/shaders/PerFragLighting_withWorldLight.frag");
//
//    shaderProgram.setUniform("ambientIntensity", vec3(0.1f, 0.1f, 0.1f));
//    shaderProgram.setUniform("lightSource.position", lightSource.position);
//    shaderProgram.setUniform("lightSource.rgbIntensity", lightSource.rgbIntensity);
//
//    glBindVertexArray(vao);
//
//    GLint position_Location = shaderProgram.getAttribLocation("vertexPosition");
//    glEnableVertexAttribArray(position_Location);
//    GLint normal_Location = shaderProgram.getAttribLocation("vertexNormal");
//    glEnableVertexAttribArray(normal_Location);

    glBindVertexArray(0);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void DrawElementsBaseVertex_Example::setupGLBuffers()
{
    glBindVertexArray(vao);
    // Copy position data to OpenGL buffer.
//    glGenBuffers(1, &vbo_vertices);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
//    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(), meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);
//    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//    // Copy normal data to OpenGL buffer.
//    glGenBuffers(1, &vbo_normals);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
//    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(), meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);
//    glVertexAttribPointer(shaderProgram.getAttribLocation("vertexNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void DrawElementsBaseVertex_Example::draw()
{
}

//---------------------------------------------------------------------------------------
void DrawElementsBaseVertex_Example::logic() {
}


//---------------------------------------------------------------------------------------
void DrawElementsBaseVertex_Example::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    CHECK_GL_ERRORS;
}
