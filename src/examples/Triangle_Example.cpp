#include <Triangle_Example.hpp>
#include <memory>

using std::shared_ptr;
using std::make_shared;

int main() {
    shared_ptr<Triangle_Example> demo = make_shared<Triangle_Example>();
    demo->setWindowTitle("Triangle Demo");
    demo->start(800, 600);

    return 0;
}

void Triangle_Example::InitializeProgram()
{
    shaderProgram.loadFromFile("../data/VertexColors.vert", "../data/VertexColors.frag");
}

void Triangle_Example::InitializeVertexBuffer()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void Triangle_Example::init()
{
    InitializeProgram();
    InitializeVertexBuffer();
}

void Triangle_Example::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.begin();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    shaderProgram.end();
}

void Triangle_Example::resize (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void Triangle_Example::cleanup() {

}

void Triangle_Example::logic() {

}

