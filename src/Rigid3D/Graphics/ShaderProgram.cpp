#include "ShaderProgram.hpp"

#include <Rigid3D/Graphics/ShaderException.hpp>
#include <Rigid3D/Graphics/GlErrorCheck.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>

#include <iostream>

#include <sstream>


namespace Rigid3D {

using glm::value_ptr;
using std::ifstream;
using std::cerr;
using std::endl;
using std::stringstream;

//------------------------------------------------------------------------------------
ShaderProgram::Shader::Shader()
    : shaderObject(0),
      filePath()
{

}

//------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram()
        : programObject(0),
          prevProgramObject(0),
          activeProgram(0)
{

}
//------------------------------------------------------------------------------------
void ShaderProgram::generateProgramObject() {
    if(programObject == 0) {
        programObject = glCreateProgram();
    }
}

//------------------------------------------------------------------------------------
void ShaderProgram::attachVertexShader(const char * filePath) {
    vertexShader.shaderObject = createShader(GL_VERTEX_SHADER);
    vertexShader.filePath = filePath;

    extractSourceCodeAndCompile(vertexShader);
}

//------------------------------------------------------------------------------------
void ShaderProgram::attachFragmentShader(const char * filePath) {
    fragmentShader.shaderObject = createShader(GL_FRAGMENT_SHADER);
    fragmentShader.filePath = filePath;

    extractSourceCodeAndCompile(fragmentShader);
}

//------------------------------------------------------------------------------------
void ShaderProgram::attachGeometryShader(const char * filePath) {
    geometryShader.shaderObject = createShader(GL_GEOMETRY_SHADER);
    geometryShader.filePath = filePath;

    extractSourceCodeAndCompile(geometryShader);
}

//------------------------------------------------------------------------------------
void ShaderProgram::extractSourceCodeAndCompile(const Shader & shader) {
    string shaderSourceCode;
    extractSourceCode(shaderSourceCode, shader.filePath);

    compileShader(shader.shaderObject, shaderSourceCode);
}

//------------------------------------------------------------------------------------
void ShaderProgram::recompileShaders() {
    extractSourceCodeAndCompile(vertexShader);
    extractSourceCodeAndCompile(fragmentShader);
    extractSourceCodeAndCompile(geometryShader);
}

//------------------------------------------------------------------------------------
/**
* Extracts source code from file located at 'filePath' and places contents into
* 'shaderSource'.
*/
void ShaderProgram::extractSourceCode(string & shaderSource, const string & filePath) {
    ifstream file;

    file.open(filePath.c_str());
    if (!file) {
        stringstream strStream;
        strStream << "Error -- Failed to open file: " << filePath << endl;
        throw ShaderException(strStream.str());
    }

    stringstream strBuffer;
    string str;

    while(file.good()) {
        getline(file, str, '\r');
        strBuffer << str;
    }
    file.close();

    strBuffer << '\0';  // Append null terminator.

    shaderSource = strBuffer.str();
}

//------------------------------------------------------------------------------------
/**
* Links attached shaders within the ShaderProgram.
*
* @note This method must be called once before calling ShaderProgram::enable(), or
* before attempting to set uniform values
*/
void ShaderProgram::link() {
    if(vertexShader.shaderObject != 0) {
        glAttachShader(programObject, vertexShader.shaderObject);
    }

    if(fragmentShader.shaderObject != 0) {
        glAttachShader(programObject, fragmentShader.shaderObject);
    }

    if(geometryShader.shaderObject != 0) {
        glAttachShader(programObject, geometryShader.shaderObject);
    }

    glLinkProgram(programObject);
    checkLinkStatus();

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
ShaderProgram::~ShaderProgram() {
    deleteShaders();
}

//------------------------------------------------------------------------------------
void ShaderProgram::deleteShaders() {
    glDeleteShader(vertexShader.shaderObject);
    glDeleteShader(fragmentShader.shaderObject);
    glDeleteShader(geometryShader.shaderObject);
    glDeleteProgram(programObject);
}

//------------------------------------------------------------------------------------
GLuint ShaderProgram::createShader(GLenum shaderType) {
    GLuint shaderId = glCreateShader(shaderType);
    CHECK_GL_ERRORS;

    return shaderId;
}

//------------------------------------------------------------------------------------
void ShaderProgram::compileShader(GLuint shaderObject, const string & shaderSourceCode) {
    const char * sourceCodeStr = shaderSourceCode.c_str();
    glShaderSource(shaderObject, 1, (const GLchar **)&sourceCodeStr, NULL);

    glCompileShader(shaderObject);
    checkCompilationStatus(shaderObject);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
void ShaderProgram::checkCompilationStatus(GLuint shaderObject) {
    GLint compileSuccess;

    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
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

//------------------------------------------------------------------------------------
void ShaderProgram::enable() const {
    glUseProgram(programObject);
    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
void ShaderProgram::disable() const {
    glUseProgram((GLuint)NULL);
    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
void ShaderProgram::checkLinkStatus() {
    GLint linkSuccess;

    glGetProgramiv(programObject, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLint errorMessageLength;
        // Get the length in chars of the link error message.
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &errorMessageLength);

        // Retrieve the link error message.
        GLchar errorMessage[errorMessageLength];
        glGetProgramInfoLog(programObject, errorMessageLength, NULL, errorMessage);

        stringstream strStream;
        strStream << "Error Linking Shaders: " << errorMessage << endl;

        throw ShaderException(strStream.str());
    }
}

//------------------------------------------------------------------------------------
/**
 *
 * @return a \c GLuint representing the program object for this \c ShaderProgram
 */
GLuint ShaderProgram::getProgramObject() const {
    return programObject;
}

//------------------------------------------------------------------------------------
/**
 * Gets the location of a uniform variable within the shader program.
 *
 * @param uniformName - string representing the name of the uniform variable.
 *
 * @return a GLint location of the requested uniform variable.
 *
 * @throws ShaderException  if \c uniformName does not correspond to an active
 * uniform variable within the shader program or if \c uniformName
 * starts with the reserved prefix "gl_".
 */
GLint ShaderProgram::getUniformLocation(const char * uniformName) const {
    GLint result = glGetUniformLocation(programObject, (const GLchar *)uniformName);

    if (result == -1) {
        stringstream errorMessage;
        errorMessage << "Error obtaining uniform location: " << uniformName;
        throw ShaderException(errorMessage.str());
    }

    return result;
}

//------------------------------------------------------------------------------------
/**
 * Gets the location of an attribute variable within the shader program.
 *
 * @param attributeName - string representing the name of the attribute variable.
 *
 * @return a GLint location of requested attribute variable.
 *
 * @throws ShaderException  if \c attributeName does not correspond to an active
 * attribute variable within the shader program or if \c attributeName
 * starts with the reserved prefix "gl_".
 */
GLint ShaderProgram::getAttribLocation(const char * attributeName) const {
    GLint result = glGetAttribLocation(programObject, (const GLchar *)attributeName);

    if (result == -1) {
        stringstream errorMessage;
        errorMessage << "Error obtaining attribute location: " << attributeName;
        throw ShaderException(errorMessage.str());
    }

    return result;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param b
 */
void ShaderProgram::setUniform(const char * uniformName, bool b) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniform1i(uniformLocation, b);
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param i
 */
void ShaderProgram::setUniform(const char * uniformName, int i) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniform1i(uniformLocation, i);
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param ui
 */
void ShaderProgram::setUniform(const char * uniformName, unsigned int ui) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniform1ui(uniformLocation, ui);
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param f
 */
void ShaderProgram::setUniform(const char * uniformName, float f) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniform1f(uniformLocation, f);
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param x
 * @param y
 */
void ShaderProgram::setUniform(const char * uniformName, float x, float y) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniform2f(uniformLocation, x, y);
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param x
 * @param y
 * @param z
 */
void ShaderProgram::setUniform(const char * uniformName, float x, float y, float z) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniform3f(uniformLocation, x, y, z);
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param x
 * @param y
 * @param z
 * @param w
 */
void ShaderProgram::setUniform(const char * uniformName, float x, float y, float z, float w) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniform4f(uniformLocation, x, y, z, w);
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param v
 */
void ShaderProgram::setUniform(const char * uniformName, const vec2 & v) {
    this->setUniform(uniformName, v.x, v.y);
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param v
 */
void ShaderProgram::setUniform(const char * uniformName, const vec3 & v) {
    this->setUniform(uniformName, v.x, v.y, v.z);
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param v
 */
void ShaderProgram::setUniform(const char * uniformName, const vec4 & v) {
    this->setUniform(uniformName, v.x, v.y, v.z, v.w);
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param m
 */
void ShaderProgram::setUniform(const char * uniformName, const mat2 & m) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m));
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param m
 */
void ShaderProgram::setUniform(const char * uniformName, const mat3 & m) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m));
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
/**
 * Set the value of a uniform variable within the shader program.
 *
 * @param uniformName - name of the uniform variable to be set.
 * @param m
 */
void ShaderProgram::setUniform(const char * uniformName, const mat4 & m) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    glUseProgram(programObject);
    GLint uniformLocation = getUniformLocation(uniformName);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m));
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

//------------------------------------------------------------------------------------
void ShaderProgram::setUniformSubroutine(GLenum shaderType, const char * subroutineName) {
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&activeProgram);
    GLuint index = glGetSubroutineIndex(programObject, shaderType, subroutineName);
    if (index == GL_INVALID_INDEX) {
        stringstream errorMessage;
        errorMessage << "Error in method ShaderProgram::setUniformSubroutine. " <<
                endl
                     << subroutineName << " is not a known subroutine.";
        throw ShaderException(errorMessage.str());
    }
    glUseProgram(programObject);
    glUniformSubroutinesuiv(shaderType, 1, &index);
    glUseProgram(activeProgram);

    CHECK_GL_ERRORS;
}

} // end namespace GlUtils
