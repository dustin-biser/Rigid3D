#include <GlfwOpenGlWindow.hpp>
#include <GlfwException.hpp>
#include <sstream>
#include <iostream>

using namespace std;

void GlfwOpenGlWindow::error_callback(int error, const char* description) {
    throw GlfwException(description);
}

void GlfwOpenGlWindow::start(int width, int height, const string & windowTitle) {
    glfwSetErrorCallback(error_callback);

    if (glfwInit() == GL_FALSE) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

    window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw GlfwException("Call to glfwCreateWindow failed.");
    }


    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyInputHandler);

    setupGl();
    init();
    centerWindow();

    while (!glfwWindowShouldClose(window)) {
        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
}

GlfwOpenGlWindow::~GlfwOpenGlWindow() {
   // Free all GLFW resources.
   glfwTerminate();
}

void GlfwOpenGlWindow::keyInputHandler(GLFWwindow* window, int key, int scancode,
        int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void GlfwOpenGlWindow::centerWindow() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (monitor == NULL) {
        return;
    }

    int x, y;
    const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);
    x = videoMode->width;
    y = videoMode->height;

    x = (x - windowWidth) / 2;
    y = (y - windowHeight) / 2;

    glfwSetWindowPos(window, x, y);
}

void GlfwOpenGlWindow::setupGl() {
    // Render only the front face of geometry.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Setup depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_DEPTH_CLAMP);

    glClearColor(0.3, 0.5, 0.7, 1.0);
}

/**
 * @brief Closes the OpenGL window and terminates the main loop.
 */
void GlfwOpenGlWindow::close() {
   glfwSetWindowShouldClose(window, GL_TRUE);
}

void GlfwOpenGlWindow::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
