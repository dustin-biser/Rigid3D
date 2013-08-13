#include <OpenGlContext.hpp>
#include <GlfwException.hpp>
#include <sstream>

using std::stringstream;

void OpenGlContext::error_callback(int error, const char* description) {
    throw GlfwException(description);
}

/**
 * Initializes the \c OpenGlContext for use.
 */
void OpenGlContext::init() {
    glfwSetErrorCallback(error_callback);

    if (glfwInit() == GL_FALSE) {
        throw GlfwException("Call to glfwInit() failed.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    window = glfwCreateWindow(600, 400, "Window Hosting OpenGL Context", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw GlfwException("Call to glfwCreateWindow failed.");
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowShouldClose(window, GL_TRUE);

    // Initialize OpenGL extensions with GLEW
    GLenum errorCode = glewInit();
    if (GLEW_OK != errorCode) {
        stringstream errorMessage;
        errorMessage << "Failed to initialize GLEW -- " << glewGetErrorString(errorCode);
        throw GlfwException(errorMessage.str());
    }
}

OpenGlContext::~OpenGlContext() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
