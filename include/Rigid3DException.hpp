#ifndef RIGID3D_EXCEPTION_HPP_
#define RIGID3D_EXCEPTION_HPP_

#include <exception>

namespace Rigid3D {

    class Rigid3DException : public std::exception {
    public:
        Rigid3DException(const char * message = "GlUtilsException Occurred.")
            : errorMessage(message) { }

        virtual ~Rigid3DException() noexcept { }

        virtual const char * what() const noexcept {
            return errorMessage;
        }

    private:
        const char * errorMessage;
    };

}

#endif /* RIGID3D_EXCEPTION_HPP_ */
