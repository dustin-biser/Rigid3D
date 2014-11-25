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
    /**
     * Constructs a \c ShaderProgram with no attached shaders.  Vertex and fragment
     * shader programs can be attached to this \c ShaderProgram by calling the
     * method \c ShaderProgram::loadFromFile.
     */
    ShaderProgram::ShaderProgram() : programObject(0), prevProgramObject(0), activeProgram(0) { }

    //------------------------------------------------------------------------------------
    /**
     * Constructs a \c ShaderProgram object from vertex shader and fragment shader
     * source files.
     * @param vertexShaderFile - location to vertex shader source file.
     * @param fragmentShaderFile - location to fragment shader source file.
     */
    ShaderProgram::ShaderProgram(const char * vertexShaderFile, const char * fragmentShaderFile)
            : programObject(0),
              prevProgramObject(0),
              activeProgram(0) {

        loadFromFile(vertexShaderFile, fragmentShaderFile);
    }

    //------------------------------------------------------------------------------------
    /**
     * Constructs a vertex and fragment shader from the files provided, compiles them,
     * and attaches them to the \c ShaderProgram for use.
     *
     * @param vertexShaderFile - path to vertex shader file.
     * @param fragmentShaderFile - path to fragment shader file.
     *
     * @throws ShaderException in the event any of the following conditions occur:
     *   - either \c vertexShaderfile or \fragmentShaderFile cannot be opened or located.
     *   - shader files contain compilation errors.
     *   - shader files result in link errors when linked together.
     */
    void ShaderProgram::loadFromFile(const char * vertexShaderFile,
            const char * fragmentShaderFile) {
        if (programObject != 0) {
            // Shader program was previously loaded, so get previous program object.
            prevProgramObject = programObject;
        }

        bool success = false;
        try {
            extractSourceCode(vertexShaderFile, vertexShader);
            extractSourceCode(fragmentShaderFile, fragmentShader);

            createShader(GL_VERTEX_SHADER);
            compileShader(vertexShader);

            createShader(GL_FRAGMENT_SHADER);
            compileShader(fragmentShader);

            createShaderProgram();
            success = true;
        } catch (const ShaderException & se) {
            if (prevProgramObject != 0) {
                // A shader program was previously loaded, so just write the
                // compile/link error to cerr and fall back to previous loaded program.
                cerr << se.what();
                programObject = prevProgramObject;
            } else {
                // This is the first time loading shader, so throw error to stop program.
                throw se;
            }
        }

        if ((success) &&
            (prevProgramObject != programObject) &&  // Necessary for when changing OpenGL Contexts
            (prevProgramObject != 0)) {

            // Delete previous program and shaders.
            GLsizei count;
            GLuint numShaders = 2;
            GLuint shaders[numShaders];
            glGetAttachedShaders(prevProgramObject, numShaders, &count, shaders);
            glDeleteShader(shaders[0]);
            glDeleteShader(shaders[1]);
            glDeleteProgram(prevProgramObject);
            prevProgramObject = 0;

            checkGLErrors(__FILE__, __LINE__);
        }

        cleanUpResources();
    }

    ShaderProgram::~ShaderProgram() {
        deleteShaders();
    }

    //------------------------------------------------------------------------------------
    void ShaderProgram::createShader(GLenum shaderType) {
        switch (shaderType) {
        case GL_VERTEX_SHADER:
            vertexShader.shaderObject = glCreateShader( GL_VERTEX_SHADER );
            break;
        case GL_FRAGMENT_SHADER:
            fragmentShader.shaderObject = glCreateShader( GL_FRAGMENT_SHADER);
            break;
        default:
            break;
        }
        checkGLErrors(__FILE__, __LINE__);
    }

    //------------------------------------------------------------------------------------
    void ShaderProgram::compileShader(const Shader &shader) {
        const char *source = shader.sourceCode.c_str();
        glShaderSource(shader.shaderObject, 1, (const GLchar **)&source, NULL);
        checkGLErrors(__FILE__, __LINE__);

        glCompileShader(shader.shaderObject);
        checkGLErrors(__FILE__, __LINE__);
        checkCompilationStatus(shader);
    }

    //------------------------------------------------------------------------------------
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
            GLchar errorMessage[errorMessageLength + 1]; // Add 1 for null terminator
            glGetShaderInfoLog(shader.shaderObject, errorMessageLength, NULL, errorMessage);

            string message = "Error Compiling Shader: ";
            message += errorMessage;

            throw ShaderException(message);
        }
    }

    //------------------------------------------------------------------------------------
    void ShaderProgram::createShaderProgram() {
        programObject = glCreateProgram();

        glAttachShader(programObject, vertexShader.shaderObject);
        glAttachShader(programObject, fragmentShader.shaderObject);

        checkGLErrors(__FILE__, __LINE__);

        glLinkProgram(programObject);
        checkLinkStatus();
    }

    void ShaderProgram::enable() {
        glUseProgram(programObject);
        checkGLErrors;
    }

    void ShaderProgram::disable() {
        glUseProgram((GLuint)NULL);
        checkGLErrors;
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
     * Extract text from \c sourceFile and store it in the field \c shader.sourceCode.
     *
     * @param sourceFileName
     * @param shader
     *
     * @throws ShaderException if \c sourceFile cannot be found or opened for reading.
     */
    void ShaderProgram::extractSourceCode(const char * sourceFile, Shader &shader) {
        ifstream file;

        file.open(sourceFile);
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
    void ShaderProgram::cleanUpResources() {
        vertexShader.sourceCode.clear();
        fragmentShader.sourceCode.clear();
    }

    //------------------------------------------------------------------------------------
    void ShaderProgram::deleteShaders() {
        glDeleteShader(vertexShader.shaderObject);
        glDeleteShader(fragmentShader.shaderObject);
        glDeleteProgram(programObject);
    }

    //------------------------------------------------------------------------------------
    void ShaderProgram::setUniformSubroutine(GLenum shaderType, const char * subroutineName) {
        GLuint index = glGetSubroutineIndex(programObject, shaderType, subroutineName);
        if (index == GL_INVALID_INDEX) {
            stringstream errorMessage;
            errorMessage << "Error in method ShaderProgram::setUniformSubroutinesuiv." << endl
                         << subroutineName << " is not a known subroutine.";
            throw ShaderException(errorMessage.str());
        }
        glUseProgram(programObject);
        glUniformSubroutinesuiv(shaderType, 1, &index);
        glUseProgram(activeProgram);

        CHECK_GL_ERRORS;
    }

} // end namespace GlUtils
