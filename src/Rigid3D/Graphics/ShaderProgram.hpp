#ifndef RIGID3D_SHADER_PROGRAM_HPP_
#define RIGID3D_SHADER_PROGRAM_HPP_

#include <Rigid3D/Common/Settings.hpp>

#include <OpenGL/gl3.h>

#include <string>


namespace Rigid3D {

    class ShaderProgram {
    public:
        ShaderProgram();

        ~ShaderProgram();

        void generateProgramObject();

        void attachVertexShader(const char * filePath);
        void attachFragmentShader(const char * filePath);
        void attachGeometryShader(const char * filePath);

        void link();

        void enable() const;

        void disable() const;

        void recompileShaders();

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
            GLuint shaderObject;
            std::string filePath;

            Shader();
        };

        Shader vertexShader;
        Shader fragmentShader;
        Shader geometryShader;

        GLuint programObject;
        GLuint prevProgramObject;
        GLuint activeProgram;

        void extractSourceCode(std::string & shaderSource, const std::string & filePath);
        void extractSourceCodeAndCompile(const Shader &shader);

        GLuint createShader(GLenum shaderType);

        void compileShader(GLuint shaderObject, const std::string & shader);

        void checkCompilationStatus(GLuint shaderObject);

        void checkLinkStatus();

        void deleteShaders();

    };

} // end namespace Rigid3D

#endif /* RIGID3D_SHADER_PROGRAM_HPP_ */
