#include "Shader.hpp"
#include "ShaderException.hpp"
#include "GlErrorCheck.hpp"
#include "glcorearb.h"

#include <fstream>
#include <sstream>

namespace Rigid3D {

using std::ifstream;
using std::stringstream;
using std::endl;

//----------------------------------------------------------------------------------------
Shader::Shader()
    : shaderObject(0),
      filePathName(nullptr)
{

}

//----------------------------------------------------------------------------------------
Shader::Shader(GLenum shaderType, const char * filePathName)
    : filePathName(filePathName)
{
    shaderObject = createShader(shaderType);

    extractSourceAndCompile();
}

//------------------------------------------------------------------------------------
GLuint Shader::createShader(GLenum shaderType) {
    GLuint shaderId = glCreateShader(shaderType);
    CHECK_GL_ERRORS;

    return shaderId;
}

//------------------------------------------------------------------------------------
void Shader::extractSourceAndCompile() {
    string shaderSource;
    extractSourceCode(shaderSource, filePathName);

    compileShader(shaderSource);
}

//------------------------------------------------------------------------------------
void Shader::recompile() {
    extractSourceAndCompile();
}

//------------------------------------------------------------------------------------
void Shader::extractSourceCode(string & shaderSource, const char * filePathName) const {
    ifstream file;

    file.open(filePathName);
    if (!file) {
        stringstream strStream;
        strStream << "Error -- Failed to open file: " << filePathName << endl;
        throw ShaderException(strStream.str());
    }

    stringstream strBuffer;
    string str;

    while(file.good()) {
        getline(file, str, '\r');
        strBuffer << str;
    }
    file.close();

    // Append null terminator, so OpenGL can locate the end of string automatically
    // when calling glShaderSource(...).
    strBuffer << '\0';

    shaderSource = strBuffer.str();
}

//------------------------------------------------------------------------------------
void Shader::compileShader(const string & shaderSource) {
    const char * source = shaderSource.c_str();
    glShaderSource(shaderObject, 1, (const GLchar **)&source, NULL);

    glCompileShader(shaderObject);
    checkCompilationStatus();

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
void Shader::checkCompilationStatus() const {
    GLint compileResult;

    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        GLint errorMessageLength;
        // Get the length in chars of the compilation error message.
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &errorMessageLength);

        // Retrieve the compilation error message.
        GLchar errorMessage[errorMessageLength + 1]; // Add 1 for null terminator
        glGetShaderInfoLog(shaderObject, errorMessageLength, NULL, errorMessage);

        string message = "Error Compiling Shader: ";
        message += errorMessage;

        throw ShaderException(message);
    }
}

} // end namespace Rigid3D
