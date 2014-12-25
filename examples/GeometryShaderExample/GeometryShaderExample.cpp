#include "GeometryShaderExample.hpp"

using namespace std;

//---------------------------------------------------------------------------------------
int main() {
    shared_ptr<GlfwOpenGlWindow> demo = GeometryShaderExample::getInstance();
    demo->create(1024, 768, "Geometry Shader Demo");


    return 0;
}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> GeometryShaderExample::getInstance() {
    static GlfwOpenGlWindow * instance = new GeometryShaderExample();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
GeometryShaderExample::GeometryShaderExample() {

}

//---------------------------------------------------------------------------------------
void GeometryShaderExample::init() {
    glGenVertexArrays(1, &vao);

    setupVboData();
    setupShaders();


    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void GeometryShaderExample::setupShaders() {
    shaderProgram.attachVertexShader(
            "examples/GeometryShaderExample/shaders/PassThrough.vs");
    shaderProgram.attachFragmentShader(
            "examples/GeometryShaderExample/shaders/Color.fs");
    shaderProgram.attachGeometryShader(
            "examples/GeometryShaderExample/shaders/PassThrough.gs");
    shaderProgram.link();


    shaderProgram.setUniform("u_color", vec4(1.0f, 0.0f, 0.0f, 0.0f));
}


//---------------------------------------------------------------------------------------
void GeometryShaderExample::setupVboData() {

    float points[] = {
            // Position
            -0.45f,  0.45f,
            0.45f,  0.45f,
            0.45f, -0.45f,
            -0.45f, -0.45f,
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);


    glBindVertexArray(vao);
    {
        glEnableVertexAttribArray(position_attrib);
        glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        unsigned short indices[] = {
                0, 1, 2, 3
        };

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void GeometryShaderExample::logic() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void GeometryShaderExample::draw() {
    glBindVertexArray(vao);

    shaderProgram.enable();
    glDrawElements(GL_POINTS, 4, GL_UNSIGNED_SHORT, NULL);
    shaderProgram.disable();

    CHECK_GL_ERRORS;
}
