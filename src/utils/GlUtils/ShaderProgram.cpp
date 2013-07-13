#include <ShaderProgram.hpp>
#include <fstream>

using namespace std;

//-----------------------------------------------------------------------------
bool ShaderProgram::CheckGLError(char *file, int line) {
   GLenum glErr;
   bool errorFound = false;

   glErr = glGetError();

   // Write all errors to std::cerr stream until no more errors are genearted.
   while (glErr != GL_NO_ERROR) {
     const GLubyte* sError = gluErrorString(glErr);

     if (sError)
         cerr << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") "
             << " in File " << file << " at line: " << line << endl;
     else
         cerr << "GL Error #" << glErr << " (no message available)" << " in File "
                << file << " at line: " << line << endl;

     errorFound = true;
     glErr = glGetError();
   }
   return errorFound;
}

//-----------------------------------------------------------------------------
bool ShaderProgram::loadFromFile(const string &vertexShaderFile,
        const string &fragmentShaderFile) {

    extractSource(vertexShaderFile, vertexShaderSource);
    extractSource(fragmentShaderFile, fragmentShaderSource);

    // TODO finish implementation
}

//-----------------------------------------------------------------------------
void ShaderProgram::extractSource(const string &sourceFileName, string &dest) {
    // TODO finish implementation
}
