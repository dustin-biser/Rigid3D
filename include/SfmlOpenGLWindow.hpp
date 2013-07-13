#ifndef SFMLOPENGLWINDOW_HPP_
#define SFMLOPENGLWINDOW_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

using std::string;

class SfmlOpenGLWindow {

public:
    SfmlOpenGLWindow()
            : windowTitle("SfmlOpenGLWindow") {
    }

    virtual void start() final;

    virtual void start(int width, int height) final;

    virtual ~SfmlOpenGLWindow() { }

    void setWindowTitle(string windowTitle) {
        this->windowTitle = windowTitle;
    }

    virtual void setupGL();

    virtual void init() { }

    virtual void logic() { }

    virtual void draw() { }

    virtual void resize(int width, int height) { }

    virtual void cleanup() { }

protected:
    string windowTitle;

};

#endif /* SFMLWINDOW_HPP_ */
