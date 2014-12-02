#include "GlErrorCheck.hpp"

#include <Rigid3D/Common/Rigid3DException.hpp>

#include <OpenGL/gl3.h>

#include <sstream>

namespace Rigid3D {
  
using std::stringstream;
using std::endl;

// OpenGL Errors since 3.1 core
//#define GL_NO_ERROR                       0
//#define GL_INVALID_ENUM                   0x0500
//#define GL_INVALID_VALUE                  0x0501
//#define GL_INVALID_OPERATION              0x0502
//#define GL_OUT_OF_MEMORY                  0x0505
//#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT         0x8CD6
//#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
//#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER        0x8CDB
//#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER        0x8CDC
//#define GL_FRAMEBUFFER_UNSUPPORTED                   0x8CDD

string getErrorString(GLenum errorCode) {
    string result;

    switch (errorCode) {
        case GL_NO_ERROR:
            break;

        case GL_INVALID_ENUM:
            result = "GL_INVALID_ENUM";
            break;

        case GL_INVALID_VALUE:
            result = "GL_INVALID_VALUE";
            break;

        case GL_INVALID_OPERATION:
            result = "GL_INVALID_OPERATION";
            break;

        case GL_OUT_OF_MEMORY:
            result = "GL_OUT_OF_MEMORY";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            result = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            result = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            result = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            result = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            result = "GL_FRAMEBUFFER_UNSUPPORTED";
            break;

        case GL_FRAMEBUFFER_UNDEFINED:
            result = "GL_FRAMEBUFFER_UNDEFINED";
            break;

        default:
            break;
        }

    return result;
}

/**
 * Checks for OpenGL error messages.  If one or more errors exist, this method throws
 * a \c Rigid3DException error containing the OpenGL error Codes.
 */
void checkGLErrors(const string &currentFileName, int currentLine) {
  GLenum errorCode;
  bool errorFound = false;
  
  
  stringstream errorMessage;
  
  // Write all errors to errorMessage stringstream until error list is exhausted.
  do {
    errorCode = glGetError();
    
    if (errorCode != GL_NO_ERROR) {
      errorMessage << "[GL Error " << errorCode <<  " " << getErrorString(errorCode) << " "
      << " caused by " << currentFileName << ":" << currentLine << "]" << endl;
      
      errorFound = true;
    }
    errorCode = glGetError();
  } while (errorCode != GL_NO_ERROR);
  
  if (errorFound) {
    throw Rigid3DException(errorMessage.str());
  }
}

void checkFramebufferCompleteness() {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        stringstream error;
        error << "Framebuffer not complete. ";
        error << getErrorString(status);
        throw Rigid3DException(error.str());
    }
}

} // namespace Rigid3D



