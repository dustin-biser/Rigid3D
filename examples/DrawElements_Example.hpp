/**
* @brief DrawElementsBaseVertex_Example
*
* @author Dustin Biser
*/
#pragma once

#include "Utils/GlfwOpenGlWindow.hpp"

#include <Rigid3D/Rigid3D.hpp>
using namespace Rigid3D;

#include <memory>
using std::shared_ptr;

class DrawElements_Example : public GlfwOpenGlWindow {

public:
    ~DrawElements_Example() { }

    static shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    DrawElements_Example(); // Singleton. Prevent direct construction.

    ShaderProgram shaderProgram;

    GLuint vao;
    GLuint vbo;

    virtual void init();
    virtual void logic();
    virtual void draw();
    virtual void keyInput(int key, int action, int mods) { }
    virtual void cleanup();

    void setupShaders();
    void setupGLBuffers();
};
