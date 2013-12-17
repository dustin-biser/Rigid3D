/**
 * GlErrorCheck.hpp
 *
 * @brief class for checking and reporting OpenGL errors.
 *
 * @author Dustin Biser
 */

#ifndef GLERRORCHECK_HPP_
#define GLERRORCHECK_HPP_

#include <string>

namespace Rigid3D {

    void checkGLErrors(const std::string & currentFileName, int currentLineNumber);

}


#endif /* GLERRORCHECK_HPP_ */
