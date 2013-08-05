#ifndef SFMLOPENGLWINDOW_HPP_
#define SFMLOPENGLWINDOW_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

using std::string;

/**
 * @brief Abstract class for instantiating an SFML OpenGL context enabled
 * window.
 */
class SfmlOpenGLWindow {

public:
    SfmlOpenGLWindow()
            : windowTitle("SfmlOpenGLWindow"), isRunning(false) {
    }

    virtual void start();

    virtual void start(int width, int height);

    virtual ~SfmlOpenGLWindow() { }

    void setWindowTitle(string windowTitle) {
        this->windowTitle = windowTitle;
    }

    virtual void init() = 0;

    virtual void logic() = 0;

    virtual void draw() = 0;

    virtual void resize(int width, int height) = 0;

    virtual void cleanup() = 0;

    virtual void close();


protected:
    string windowTitle;
    bool isRunning;

    void centerWindow(sf::Window & window);

    void setupGL();

};

#endif /* SFMLWINDOW_HPP_ */
