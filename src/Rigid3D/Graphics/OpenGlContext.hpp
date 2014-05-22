#ifndef RIGID3D_OPENGLCONTEXT_HPP_
#define RIGID3D_OPENGLCONTEXT_HPP_

struct GLFWwindow;

namespace Rigid3D {

    /**
     * @brief Class for initializing a specific OpenGL context with major and
     * minor version numbers, prior to using any associated OpenGL function
     * calls.
     *
     * @author Dustin Biser
     */
    class OpenGLContext {
    public:
        OpenGLContext(unsigned int majorVersion, unsigned int minorVersion)
                : window(nullptr), majorVersion(majorVersion), minorVersion(minorVersion) { }

        ~OpenGLContext();

         void init();

    protected:
        GLFWwindow *window;
        unsigned int majorVersion;
        unsigned int minorVersion;

        static void error_callback(int error, const char* description);
    };

}

#endif /* RIGID3D_OPENGLCONTEXT_HPP_ */
