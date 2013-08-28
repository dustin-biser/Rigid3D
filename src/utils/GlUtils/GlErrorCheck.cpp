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

namespace GlUtils {

    /**
     * Checks for OpenGL error messages.  If one ore more errors exist, this method throws
     * a \c ShaderException error containing all GL error messages.
     *
     * @param currentFileName - name of file to reference, if errors exist.
     * @param currentLine - line number to reference within file, if errors exist.
     */
    void checkGLErrors(const string &currentFileName, int currentLine) {
       GLenum glErr;
       bool errorFound = false;

       glErr = glGetError();

       stringstream strStream;

       // Write all errors to strStream until error list is exhausted.
       while (glErr != GL_NO_ERROR) {
         const GLubyte* sError = gluErrorString(glErr);

         if (sError)
             strStream << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") "
                 << " in File " << currentFileName << " at line:" << currentLine << endl;
         else
             strStream << "GL Error #" << glErr << " (no message available)" << " in File "
                    << currentFileName << " at line:" << currentLine << endl;

         errorFound = true;
         glErr = glGetError();
       }

       if (errorFound) {
           throw ShaderException(strStream.str());
       }
    }
}



