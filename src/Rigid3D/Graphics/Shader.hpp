#pragma once

#include <Rigid3D/Common/Settings.hpp>
#include <OpenGL/gl3.h>

#include <string>

namespace Rigid3D {

class Shader {

public:
    Shader(GLenum shaderType, const char * filePathName);

    Shader();

    void recompile();

private:
    GLuint shaderObject;
    const char * filePathName;

    GLuint createShader(GLenum shaderType);

    void extractSourceAndCompile();

    void extractSourceCode(std::string & shaderSource, const char * filePathName) const;

    void compileShader(const std::string & shaderSource);

    void checkCompilationStatus() const;

};

} // end namespace Rigid3D