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
#define CHECK_FRAMEBUFFER_COMPLETENESS Rigid3D::checkFramebufferCompleteness()
#else
#define CHECK_GL_ERRORS
#define CHECK_FRAMEBUFFER_COMPLETENESS
#endif

namespace Rigid3D {

    void checkGLErrors(const std::string & currentFileName, int currentLineNumber);

    void checkFramebufferCompleteness();

}


#endif /* RIGID3D_GLERRORCHECK_HPP_ */
