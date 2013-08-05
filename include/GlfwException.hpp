/**
 * @brief Class for encapsulating GLWF errors.
 *
 * @author Dustin Biser
 */

#ifndef GLFWEXCEPTION_HPP_
#define GLFWEXCEPTION_HPP_

#include <exception>
#include <string>

using namespace std;

class GlfwException : public exception {
public:
    GlfwException(const string & message) : errorMessage("GlfwException: ") {
        errorMessage.append(message).append("\n");
    }

    virtual ~GlfwException() noexcept { }

    virtual const char * what() const noexcept {
        return errorMessage.c_str();
    }

private:
    string errorMessage;
};

#endif /* GLFWEXCEPTION_HPP_ */
