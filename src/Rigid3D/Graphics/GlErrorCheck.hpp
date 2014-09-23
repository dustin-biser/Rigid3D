/**
 * GlErrorCheck.hpp
 *
 * @brief class for checking and reporting OpenGL errors.
 *
 * @author Dustin Biser
 */

#ifndef RIGID3D_GLERRORCHECK_HPP_
#define RIGID3D_GLERRORCHECK_HPP_

#include <string>


#if(DEBUG)
#define CHECK_GL_ERRORS Rigid3D::checkGLErrors(__FILE__, __LINE__)
#else
#define CHECK_GL_ERRORS
#endif

namespace Rigid3D {

    void checkGLErrors(const std::string & currentFileName, int currentLineNumber);

}


#endif /* RIGID3D_GLERRORCHECK_HPP_ */
