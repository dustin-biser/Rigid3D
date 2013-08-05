#ifndef SHADEREXCEPTION_HPP_
#define SHADEREXCEPTION_HPP_

#include <exception>

using namespace std;

namespace GlUtils {

    class ShaderException : public exception {
    public:
        ShaderException(const string & message = "Shader Exception Occurred.")
            : errorMessage(message) { }

        virtual ~ShaderException() noexcept { }

        virtual const char * what() const noexcept {
            return errorMessage.c_str();
        }

    private:
        string errorMessage;
    };

}


#endif /* SHADEREXCEPTION_HPP_ */
