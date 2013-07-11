/*
 * Triangle.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: Dustin Biser
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include <SFML/Graphics/Shader.hpp>
#include <vector>

sf::Shader shader;

void InitializeProgram()
{
    shader.loadFromFile("../data/VertexColors.vert", "../data/VertexColors.frag");
}

const float vertexData[] = {
    0.0f,    0.5f, 0.0f, 1.0f,
    0.5f, -0.366f, 0.0f, 1.0f,
    -0.5f, -0.366f, 0.0f, 1.0f,
    1.0f,    0.0f, 0.0f, 1.0f,
    0.0f,    1.0f, 0.0f, 1.0f,
    0.0f,    0.0f, 1.0f, 1.0f,
};

GLuint vertexBufferObject;
GLuint vao;


void InitializeVertexBuffer()
{
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
    InitializeProgram();
    InitializeVertexBuffer();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void draw()
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

void resize (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void releaseResources() {

}

void logic() {

}

