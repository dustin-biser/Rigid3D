#ifndef SHADER_HPP_
#define SHADER_HPP_

// TODO Goal is to ease the use of glsl shader compilation, program linking, and error reporting.

/*
   Sample Shader Compilation + Link usage:

   // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers( 1, &vbo );

    float vertices[] = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexSource, NULL );
    glCompileShader( vertexShader );

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentSource, NULL );
    glCompileShader( fragmentShader );

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glBindFragDataLocation( shaderProgram, 0, "outColor" );
    glLinkProgram( shaderProgram );
    glUseProgram( shaderProgram );

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0 );

 */

#include <string>
#include <GL/glew.h>
#include <iostream>

using std::string;

class ShaderProgram {
public:
    bool loadFromFile(const string &vertexShaderFile, const string &fragmentShaderFile);

private:
    string vertexShaderSource;
    string fragmentShaderSource;

    bool CheckGLError(char *fileName, int lineNumber);
    void extractSource(const string &sourceFileName, string &dest);
};


#endif /* SHADER_HPP_ */
