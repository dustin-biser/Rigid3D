/**
 * @brief GlfwOpenGlWindow-Example.cpp
 *
 * @author Dustin Biser
 */

#include <GlfwOpenGlWindow.hpp>
#include <memory>

class GlfwOpenGlWindowExample : public GlfwOpenGlWindow {
public:
    GlfwOpenGlWindowExample() { }

    virtual ~GlfwOpenGlWindowExample() { }

    virtual void resize(int width, int height) { }

    virtual void close() {}

protected:
    virtual void init() { }

    virtual void cleanup() { }

};

int main() {
    shared_ptr<GlfwOpenGlWindowExample> glfwWindow = make_shared<GlfwOpenGlWindowExample>();
    glfwWindow->start(800, 600, "GlfwWindow Test");

    return 0;
}
