#include <TriangleUniformColor_Example.hpp>
#include <memory>
#include <iostream>

using namespace std;

int main() {
    try{
        shared_ptr<TriangleUniformColor_Example> demo = make_shared<TriangleUniformColor_Example>();
        demo->setWindowTitle("Triangle Demo");
        demo->start(800, 600);
    } catch (const exception & e) {
        cerr << e.what() << endl;
    }

    return 0;
}

void TriangleUniformColor_Example::initShaders()
{
    shaderProgram.loadFromFile("../data/UniformVertexColors.vert", "../data/UniformVertexColors.frag");
}

void TriangleUniformColor_Example::initGLData()
{
    // Generate and bind vertex array object
    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

    GLint positionAttribLocation = shaderProgram.getAttribLocation("position");
    GLint colorUniformLocation = shaderProgram.getUniformLocation("color");

    glEnableVertexAttribArray(positionAttribLocation);
    glVertexAttribPointer(positionAttribLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

    shaderProgram.enable();
        // Set uniform color for the triangle
        float colorVec4[] = {0.7f, 0.2f, 0.2f, 1.0f};
        glUniform4fv(colorUniformLocation, 1, colorVec4);
    shaderProgram.disable();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(vao);
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void TriangleUniformColor_Example::init()
{
    initShaders();
    initGLData();
}

void TriangleUniformColor_Example::draw()
{
    glBindVertexArray(vao);

    shaderProgram.begin();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
    shaderProgram.end();

    glBindVertexArray(0);
}

void TriangleUniformColor_Example::resize (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void TriangleUniformColor_Example::cleanup() {
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &vao);
}

void TriangleUniformColor_Example::logic() {

}

