#include "DrawElements_Example.hpp"

int main() {
    shared_ptr<GlfwOpenGlWindow> demo = DrawElements_Example::getInstance();
    demo->create(800, 600, "DrawElements Example");

    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> DrawElements_Example::getInstance() {
    static GlfwOpenGlWindow * instance = new DrawElements_Example();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
DrawElements_Example::DrawElements_Example()
    : vao(0), vbo(0) {
}

//---------------------------------------------------------------------------------------
void DrawElements_Example::init()
{
    glGenVertexArrays(1, &vao);

    setupShaders();
    setupGLBuffers();

    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
}

//---------------------------------------------------------------------------------------
void DrawElements_Example::setupShaders() {

    glBindVertexArray(0);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void DrawElements_Example::setupGLBuffers()
{
    float32 vertices[] = { -1.0f, 0.0f, 0.0f,
                           1.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f };

    // TODO Dustin - Use the above vertex data to create multiple triangles.


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
void DrawElements_Example::draw()
{
}

//---------------------------------------------------------------------------------------
void DrawElements_Example::logic() {
}


//---------------------------------------------------------------------------------------
void DrawElements_Example::cleanup() {
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    CHECK_GL_ERRORS;
}
