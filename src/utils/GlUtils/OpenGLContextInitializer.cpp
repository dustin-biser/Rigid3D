#include <OpenGLContexInitializer.hpp>
#include <sstream>

using namespace GlUtils;

/**
 * Initializes an OpenGL context with \c majorVersion and \c minorVersion using
 * SFML and GLEW libraries.
 *
 * @param majorVersion
 * @param minorVersion
 */
void OpenGLContextInitializer::initContext() {
    //-- Initialize OpenGL context using SFML and GLEW
    contextSettings = make_shared<sf::ContextSettings>();
    contextSettings->depthBits = 24;
    contextSettings->stencilBits = 8;
    contextSettings->antialiasingLevel = 2;
    contextSettings->majorVersion = this->majorVersion;
    contextSettings->minorVersion = this->minorVersion;

    // Create the window
    openGL_window = make_shared<sf::Window>(sf::VideoMode(600, 400), "Test Window", sf::Style::Default, *contextSettings);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        stringstream errorMessage;
        errorMessage << "Error calling glewInit(): OpenGL context is not valid.";
        throw GlUtilsException(errorMessage.str());
    }
}
