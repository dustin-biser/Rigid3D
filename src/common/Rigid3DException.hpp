#ifndef RIGID3D_EXCEPTION_HPP_
#define RIGID3D_EXCEPTION_HPP_

#include <exception>

namespace Rigid3D {

    class Rigid3DException : public std::exception {
    public:
        Rigid3DException(const string & message)
            : errorMessage(message) { }

        virtual ~Rigid3DException() noexcept { }

        virtual const char * what() const noexcept {
            return errorMessage.c_str();
        }

    private:
        string errorMessage;
    };

}

#endif /* RIGID3D_EXCEPTION_HPP_ */
