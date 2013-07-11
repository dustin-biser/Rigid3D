#ifndef SFMLOPENGLWINDOW_HPP_
#define SFMLOPENGLWINDOW_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

using std::string;

class SfmlOpenGLWindow {
    protected: string windowTitle;

    public: SfmlOpenGLWindow() : windowTitle("SfmlOpenGLWindow") { }

    public: virtual void start() final;

    public: virtual void start(int width, int height) final;

    public: virtual ~SfmlOpenGLWindow() { }

    public: void setWindowTitle(string windowTitle) {
        this->windowTitle = windowTitle;
    }

    public: virtual void setupGL();

    public: virtual void init();

    public: virtual void logic();

    public: virtual void draw();

    public: virtual void resize(int width, int height);

    public: virtual void cleanup();
};

#endif /* SFMLWINDOW_HPP_ */
