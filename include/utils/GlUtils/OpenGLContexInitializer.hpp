/**
 * OpenGLContexInitializer
 *
 * @brief Use this class for initializing an OpenGL context, prior to using
 * OpenGL function calls.
 *
 * @author Dustin Biser
 *
 */

#ifndef OPENGLCONTEXINITIALIZER_HPP_
#define OPENGLCONTEXINITIALIZER_HPP_

// Used to setup OpenGL rendering context
#include <GlUtils.hpp>
#include <memory>

using namespace std;

namespace GlUtils {

    class OpenGLContextInitializer {
    protected:
        shared_ptr<sf::Window>  openGL_window;
        shared_ptr<sf::ContextSettings> contextSettings;
        unsigned int majorVersion;
        unsigned int minorVersion;

    public:
        OpenGLContextInitializer(unsigned int majorVersion, unsigned int minorVersion)
                : majorVersion(majorVersion), minorVersion(minorVersion) { }

        void initContext();
    };

} // end namespace GlUtils

#endif /* OPENGLCONTEXINITIALIZER_HPP_ */
