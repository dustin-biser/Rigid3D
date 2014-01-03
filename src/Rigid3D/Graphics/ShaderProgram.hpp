#ifndef RIGID3D_SHADER_PROGRAM_HPP_
#define RIGID3D_SHADER_PROGRAM_HPP_

#include <Rigid3D/Common/Settings.hpp>

#include <GL/glew.h>

#include <string>


namespace Rigid3D {

using std::string;

    /**
     * A ShaderProgram encapsulates the compilation, linkage, and usage of both a
     * vertex shader and fragment shader with respect to an OpenGL program.
     *
     * The goal of the ShaderProgram class is to make loading and using shader
     * programs as painless as possible.
     *
     * Example usage:
     *
     * \code{.cpp}
     *  ShaderProgram shaderProgram;
     *  shaderProgram.loadFromFile("verexShaderFile", "fragmentShaderFile");
     *
     *  shaderProgram.begin();
     *      glDraw*(...); // Call OpenGL drawing command here.
     *  shaderProgram.end();
     * \endcode
     *
     * @note Attached vertex shaders must have present the following input
     * variables with given types and layout locations:
     * \code
     * layout (location = 0) in vec3 vertexPosition;
     * layout (location = 1) in vec3 vertexNormal;
     * layout (location = 2) in vec2 vertexTextureCoord;
     * layout (location = 3) in vec3 vertexColor;
     * \endcode
     * There are no restrictions imposed on the naming of these input variables
     * within the attached vertex shader.
     *
     * @note Attached vertex shaders must have present the following uniforms with
     * given types and layout locations:
     * \code
     * layout (location = 0) mat4 ModelViewMatrix
     * layout (location = 1) mat3 NormalMatrix
     * layout (location = 2) mat4 ProjectionMatrix
     * \endcode
     * There are no restrictions imposed on the naming of these uniform matrices
     * within the vertex shader.
     */
    class ShaderProgram {
    public:
        ShaderProgram();

        ShaderProgram(const char * ShaderFile, const char * fragmentShaderFile);

        ~ShaderProgram();

        void loadFromFile(const char * vertexShaderFile, const char * fragmentShaderFile);

        void enable();

        void disable();

        GLuint getProgramObject() const;

        GLint getUniformLocation(const char * uniformName) const;

        GLint getAttribLocation(const char * attributeName) const;

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

        void setUniformSubroutine(GLenum shaderType, const char * subroutineName);


    private:
        struct Shader {
            string sourceCode;
            GLuint shaderObject;
        };

        Shader vertexShader;
        Shader fragmentShader;
        GLuint programObject;
        GLuint prevProgramObject;
        GLuint activeProgram;

        void extractSourceCode(const char * sourceFileName, Shader &shader);

        void createShader(GLenum shaderType);

        void compileShader(const Shader &shader);

        void checkCompilationStatus(const Shader &shader);

        void createShaderProgram();

        void checkLinkStatus();

        void cleanUpResources();

        void deleteShaders();

    };

} // end namespace Rigid3D

#endif /* RIGID3D_SHADER_PROGRAM_HPP_ */
