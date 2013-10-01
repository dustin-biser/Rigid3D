#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <GL/glew.h>
#include <GlErrorCheck.hpp>
#include <glm/glm.hpp>
#include <string>

using std::string;
using namespace glm;

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

        ShaderProgram(const char * ShaderFile, const char * fragmentShaderFile);

        ~ShaderProgram();

        void loadFromFile(const char * vertexShaderFile, const char * fragmentShaderFile);

        void enable();

        void disable();

        GLuint getProgramObject();

        GLint getUniformLocation(const char * uniformName);

        GLint getAttribLocation(const char * attributeName);

        void setUniform(const char * uniformName, bool b);

        void setUniform(const char * uniformName, int i);

        void setUniform(const char * uniformName, unsigned int i);

        void setUniform(const char * uniformName, float f);

        void setUniform(const char * uniformName, float x, float y);

        void setUniform(const char * uniformName, float x, float y, float z);

        void setUniform(const char * uniformName, float x, float y, float z, float w);

        void setUniform(const char * uniformName, const vec2 & v);

        void setUniform(const char * uniformName, const vec3 & v);

        void setUniform(const char * uniformName, const vec4 & v);

        void setUniform(const char * uniformName, const mat2 & m);

        void setUniform(const char * uniformName, const mat3 & m);

        void setUniform(const char * uniformName, const mat4 & m);

    private:
        struct Shader {
            string sourceCode;
            GLuint shaderObject;
        };

        Shader vertexShader;
        Shader fragmentShader;
        GLuint programObject;
        GLuint activeProgram;

        void extractSourceCode(const char * sourceFileName, Shader &shader);

        void createVertexShader();

        void createFragmentShader();

        void compileShader(const Shader &shader);

        void checkCompilationStatus(const Shader &shader);

        void createShaderProgram();

        void checkLinkStatus();

        void cleanUpResources();

        void setActiveProgram();
    };

} // end namespace GlUtils

#endif /* SHADER_HPP_ */
