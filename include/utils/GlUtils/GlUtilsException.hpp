#ifndef GLUTILSEXCEPTION_HPP_
#define GLUTILSEXCEPTION_HPP_

#include <exception>

namespace GlUtils {

    class GlUtilsException : public std::exception {
    public:
        GlUtilsException(const char * message = "GlUtilsException Occurred.")
            : errorMessage(message) { }

        virtual ~GlUtilsException() noexcept { }

        virtual const char * what() const noexcept {
            return errorMessage;
        }

    private:
        const char * errorMessage;
    };

}

#endif /* GLUTILSEXCEPTION_HPP_ */
