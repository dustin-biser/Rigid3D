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

} // namespace Rigid3D



