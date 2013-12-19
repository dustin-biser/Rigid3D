/**
 * GlErrorCheck.cpp
 *
 * @author Dustin Biser
 */

#include <GL/glew.h>
#include <GlErrorCheck.hpp>
#include <ShaderException.hpp>
#include <sstream>
#include <string>

using namespace std;

namespace Rigid3D {

    /**
     * Checks for OpenGL error messages.  If one ore more errors exist, this method throws
     * a \c ShaderException error containing all GL error messages.
     *
     * @param currentFileName - name of file to reference, if errors exist.
     * @param currentLine - line number to reference within file, if errors exist.
     */
    void checkGLErrors(const string &currentFileName, int currentLine) {
       GLenum errorType;
       bool errorFound = false;

       errorType = glGetError();

       stringstream errorMessage;

       // Write all errors to strStream until error list is exhausted.
       while (errorType != GL_NO_ERROR) {
         const GLubyte* errorString = gluErrorString(errorType);

         if (errorString)
             errorMessage << "GL Error #" << errorType << "(" << gluErrorString(errorType) << ") "
                 << " in File " << currentFileName << " at line:" << currentLine << endl;
         else
             errorMessage << "GL Error #" << errorType << " (no message available)" << " in File "
                    << currentFileName << " at line:" << currentLine << endl;

         errorFound = true;
         errorType = glGetError();
       }

       if (errorFound) {
           throw ShaderException(errorMessage.str());
       }
    }
}



