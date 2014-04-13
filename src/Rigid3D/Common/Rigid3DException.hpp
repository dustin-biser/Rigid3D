#ifndef RIGID3D_EXCEPTION_HPP_
#define RIGID3D_EXCEPTION_HPP_

#include <exception>
#include <string>

namespace Rigid3D {
  
  using std::string;
  
  class Rigid3DException : public std::exception {
  public:
  
    // Make a copy of the message.
    Rigid3DException(string message)
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
