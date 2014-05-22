#include "OpenGLContext.hpp"

#include <GLFW/glfw3.h>
#include <Rigid3D/Graphics/GlfwException.hpp>
#include <sstream>

namespace Rigid3D {

using std::stringstream;

//----------------------------------------------------------------------------------------
void OpenGLContext::error_callback(int error, const char* description) {
    throw GlfwException(description);
}

//----------------------------------------------------------------------------------------
/**
 * Initializes the \c OpenGLContext for use.
 */
void OpenGLContext::init() {
    glfwSetErrorCallback(error_callback);

    if (glfwInit() == GL_FALSE) {
        throw GlfwException("Call to glfwInit() failed.");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    window = glfwCreateWindow(600, 400, "Window Hosting OpenGL Context", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw GlfwException("Call to glfwCreateWindow failed.");
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowShouldClose(window, GL_TRUE);

    // Clear error buffer.  Specifically due to glewInit() causing a
    // GLError(invalid enumerant), which is safe to ignore.
    // http://www.opengl.org/wiki/OpenGL_Loading_Library
    while(glGetError() != GL_NO_ERROR);
}

//----------------------------------------------------------------------------------------
OpenGLContext::~OpenGLContext() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

} // end namespace Rigid3D
