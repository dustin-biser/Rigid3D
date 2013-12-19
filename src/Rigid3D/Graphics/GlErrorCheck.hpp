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

namespace Rigid3D {

using std::string;

    void checkGLErrors(const string & currentFileName, int currentLineNumber);

}


#endif /* RIGID3D_GLERRORCHECK_HPP_ */
