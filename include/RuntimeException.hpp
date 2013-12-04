#ifndef RUNTIME_EXCEPTION_HPP_
#define RUNTIME_EXCEPTION_HPP_

#include <exception>

namespace Rigid3D {

class RuntimeException : public std::exception {
public:
    RuntimeException(const char * message = "RuntimeException Occurred.")
        : errorMessage(message) { }

    virtual ~RuntimeException() noexcept { }

    virtual const char * what() const noexcept {
        return errorMessage;
    }

private:
    const char * errorMessage;
};

}

#endif /* RUNTIME_EXCEPTION_HPP_ */
