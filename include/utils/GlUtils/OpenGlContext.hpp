#ifndef OPENGLCONTEXT_HPP_
#define OPENGLCONTEXT_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @brief class for initializing a specific OpenGL context, prior to using
 * any associated OpenGL function calls.
 *
 * @author Dustin Biser
 */
class OpenGlContext {
protected:
    GLFWwindow *window;
    unsigned int majorVersion;
    unsigned int minorVersion;

public:
    OpenGlContext(unsigned int majorVersion, unsigned int minorVersion)
            : window(nullptr), majorVersion(majorVersion), minorVersion(minorVersion) { }

    ~OpenGlContext();

     void init();
};


#endif /* OPENGLCONTEXT_HPP_ */
