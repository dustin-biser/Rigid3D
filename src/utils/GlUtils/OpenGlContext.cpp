#include <OpenGlContext.hpp>
#include <GlfwException.hpp>

/**
 * Initializes the \c OpenGlContext for use.
 */
void OpenGlContext::init() {
    if (glfwInit() == GL_FALSE) {
        throw GlfwException("Call to glfwInit() failed.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    window = glfwCreateWindow(100, 100, "Window Hosting OpenGL Context", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw GlfwException("Call to glfwCreateWindow failed.");
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowShouldClose(window, GL_TRUE);
}

OpenGlContext::~OpenGlContext() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
