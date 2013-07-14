#include <ShaderProgram.hpp>
#include <ShaderException.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//-----------------------------------------------------------------------------
/**
 * Constructs a \c ShaderProgram with no attached shaders.  Vertex and fragment
 * shader programs can be attached to this \c ShaderProgram by calling the
 * method \c loadFromFile(...).
 */
ShaderProgram::ShaderProgram() {
    initializeShaders();
}

//-----------------------------------------------------------------------------
/**
 * Constructs a \c ShaderProgram object from vertex shader and fragment shader
 * source files.
 * @param vertexShaderFile - location to vertex shader source file.
 * @param fragmentShaderFile - location to fragment shader source file.
 */
ShaderProgram::ShaderProgram(const string &vertexShaderFile, const string &fragmentShaderFile) {
    initializeShaders();

    loadFromFile(vertexShaderFile, fragmentShaderFile);
}

//-----------------------------------------------------------------------------
void ShaderProgram::initializeShaders() {
    vertexShader = Shader();
    fragmentShader = Shader();
}

//-----------------------------------------------------------------------------
/**
 * Constructs a vertex and fragment shader from the files provided, compiles them,
 * and attaches them to the \c ShaderProgram for use.
 *
 * A \c ShaderException will be thrown in the event any of the following conditions occur:
 *   - shader file cannot be opened or located.
 *   - shaders contain compilation errors.
 *   - shaders result in link errors when linked together.
 *
 * @param vertexShaderFile - path to vertex shader file.
 * @param fragmentShaderFile - path to fragment shader file.
 */
void ShaderProgram::loadFromFile(const string &vertexShaderFile,
        const string &fragmentShaderFile) {
    extractSourceCode(vertexShaderFile, vertexShader);
    extractSourceCode(fragmentShaderFile, fragmentShader);

    createVertexShader();
    compileShader(vertexShader);

    createFragmentShader();
    compileShader(fragmentShader);

    createShaderProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteShader(vertexShader.shaderObject);
    glDeleteShader(fragmentShader.shaderObject);
    glDeleteProgram(programObject);
}

//-----------------------------------------------------------------------------
void ShaderProgram::createVertexShader() {
    vertexShader.shaderObject = glCreateShader( GL_VERTEX_SHADER );
    checkGLError(__FILE__, __LINE__);
}

//-----------------------------------------------------------------------------
void ShaderProgram::createFragmentShader() {
    fragmentShader.shaderObject = glCreateShader( GL_FRAGMENT_SHADER);
    checkGLError(__FILE__, __LINE__);
}

//-----------------------------------------------------------------------------
void ShaderProgram::compileShader(const Shader &shader) {
    const char *source = shader.sourceCode.c_str();
    glShaderSource(shader.shaderObject, 1, (const GLchar **)&source, NULL);
    checkGLError(__FILE__, __LINE__);

    glCompileShader(shader.shaderObject);
    checkGLError(__FILE__, __LINE__);
    checkCompilationStatus(shader);
}

//-----------------------------------------------------------------------------
/**
 * Checks for shader compilation errors. If found, a \c ShaderException is thrown
 * containing the compilation error messages.
 *
 * @param shader - \c Shader object.
 */
void ShaderProgram::checkCompilationStatus(const Shader &shader) {
    GLint compileSuccess;

    glGetShaderiv(shader.shaderObject, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLint errorMessageLength;
        // Get the length in chars of the compilation error message.
        glGetShaderiv(shader.shaderObject, GL_INFO_LOG_LENGTH, &errorMessageLength);

        // Retrieve the compilation error message.
        GLchar errorMessage[errorMessageLength];
        glGetShaderInfoLog(shader.shaderObject, errorMessageLength, NULL, errorMessage);

        throw ShaderException(errorMessage);
    }
}

//-----------------------------------------------------------------------------
void ShaderProgram::createShaderProgram() {
    programObject = glCreateProgram();
    checkGLError(__FILE__, __LINE__);

    glAttachShader(programObject, vertexShader.shaderObject);
    glAttachShader(programObject, fragmentShader.shaderObject);

    glLinkProgram(programObject);
    checkLinkStatus();
}

void ShaderProgram::begin() {
    glUseProgram(programObject);
}

void ShaderProgram::end() {
    glUseProgram((GLuint)NULL);
}

//-----------------------------------------------------------------------------
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

        throw ShaderException(errorMessage);
    }
}

//-----------------------------------------------------------------------------
/**
 *
 * @return a \c GLuint reference to an OpenGL program object
 */
GLuint ShaderProgram::getProgramObject() {
    return programObject;
}

//-----------------------------------------------------------------------------
/**
 * Extract text from \c sourceFile and store it in the field \c shader.sourceCode.
 *
 * @param sourceFileName
 * @param shader
 */
void ShaderProgram::extractSourceCode(const string &sourceFile, Shader &shader) {
    ifstream file;

    file.open(sourceFile.c_str());
    if (!file) {
        stringstream strStream;
        strStream << "Error -- Failed to open file: " << sourceFile << endl;
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

    shader.sourceCode = strBuffer.str();
}

//-----------------------------------------------------------------------------
/**
 * Checks for GLU error messages.  If one ore more errors exist, this method throws
 * a \c ShaderException error containing all GLU error messages.
 *
 * @param file - name of file to reference, if errors exist.
 * @param line - line number to reference within file, if errors exist.
 */
void ShaderProgram::checkGLError(const string &file, int line) {
   GLenum glErr;
   bool errorFound = false;

   glErr = glGetError();

   stringstream strStream;

   // Write all errors to strStream until error list is exhausted.
   while (glErr != GL_NO_ERROR) {
     const GLubyte* sError = glewGetErrorString(glErr);

     if (sError)
         strStream << "GL Error #" << glErr << "(" << glewGetErrorString(glErr) << ") "
             << " in File " << file << " at line: " << line << endl;
     else
         strStream << "GL Error #" << glErr << " (no message available)" << " in File "
                << file << " at line: " << line << endl;

     errorFound = true;
     glErr = glGetError();
   }

   if (errorFound) {
       throw ShaderException(strStream.str());
   }
}
