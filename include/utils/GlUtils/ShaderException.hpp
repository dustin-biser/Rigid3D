#ifndef SHADEREXCEPTION_HPP_
#define SHADEREXCEPTION_HPP_

#include <exception>

namespace GlUtils {

    class ShaderException : public std::exception {
    public:
        ShaderException(const char * message = "Shader Exception Occurred.")
            : errorMessage(message) { }

        virtual ~ShaderException() noexcept { }

        virtual const char * what() const noexcept {
            return errorMessage;
        }

    private:
        const char * errorMessage;
    };

}


#endif /* SHADEREXCEPTION_HPP_ */
