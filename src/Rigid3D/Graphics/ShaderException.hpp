#ifndef RIGID3D_SHADEREXCEPTION_HPP_
#define RIGID3D_SHADEREXCEPTION_HPP_

#include <exception>
#include <string>

namespace Rigid3D {

using std::string;

    class ShaderException : public std::exception {
    public:
        ShaderException(const string & message)
            : errorMessage(message) { }

        virtual ~ShaderException() noexcept { }

        virtual const char * what() const noexcept {
            return errorMessage.c_str();
        }

    private:
        string errorMessage;
    };

}

#endif /* RIGID3D_SHADEREXCEPTION_HPP_ */
