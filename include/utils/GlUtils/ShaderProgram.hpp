#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <GL/glew.h>
#include <GlErrorCheck.hpp>
#include <string>

using std::string;

namespace GlUtils {

    /**
     * A \c ShaderProgram encapsulates the compilation, linkage, and usage of both a
     * vertex shader and fragment shader with respect to a single OpenGL program
     * object.
     *
     * The goal of the \c ShaderProgram class is to make loading and using shader
     * programs as painless as possible.
     *
     * Example usage:
     *
     * \code{.cpp}
     *  ShaderProgram shaderProgram;
     *  shaderProgram.loadFromFile("verexShaderFile", "fragmentShaderFile");
     *
     *  shaderProgram.begin();  // calls glUseProgram(...)
     *   ... glDraw*();
     *  shaderProgram.end();    // calls glUseProgram(NULL)
     * \endcode
     */
    class ShaderProgram {
    public:
        ShaderProgram();

        ShaderProgram(const string &vertexShaderFile, const string &fragmentShaderFile);

        ~ShaderProgram();

        void loadFromFile(const string &vertexShaderFile, const string &fragmentShaderFile);

        void enable();

        void disable();

        GLuint getProgramObject();

        GLint getUniformLocation(const string &uniformName);

        GLint getAttribLocation(const string &attributeName);

    private:
        struct Shader {
            string sourceCode;
            GLuint shaderObject;
        };

        Shader vertexShader;
        Shader fragmentShader;
        GLuint programObject;

        void initializeShaders();

        void extractSourceCode(const string &sourceFileName, Shader &shader);

        void createVertexShader();

        void createFragmentShader();

        void compileShader(const Shader &shader);

        void checkCompilationStatus(const Shader &shader);

        void createShaderProgram();

        void checkLinkStatus();
    };

} // end namespace GlUtils

#endif /* SHADER_HPP_ */
