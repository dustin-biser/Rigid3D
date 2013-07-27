#ifndef GLUTILSEXCEPTION_HPP_
#define GLUTILSEXCEPTION_HPP_

#include <exception>

using namespace std;

namespace GlUtils {

    class GlUtilsException : public exception {
    public:
        GlUtilsException(const string message = "GlUtilsException Occurred.")
            : errorMessage(message) { }

        virtual ~GlUtilsException() noexcept { }

        virtual const char * what() const noexcept {
            return errorMessage.c_str();
        }

    private:
        string errorMessage;
    };

}


#endif /* GLUTILSEXCEPTION_HPP_ */
