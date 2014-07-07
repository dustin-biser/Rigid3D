/**
 * @brief GlfwOpenGlWindow-Example.cpp
 *
 * @author Dustin Biser
 */

#include <GlfwOpenGlWindow.hpp>
#include <memory>
#include <boost/utility.hpp>

#include <iostream>
using namespace std;

///@brief Singleton
class GlfwOpenGlWindowExample : public GlfwOpenGlWindow, private noncopyable {
public:
    virtual ~GlfwOpenGlWindowExample() { }

    virtual void resize(int width, int height) { }

    virtual void close() {}

    static shared_ptr<GlfwOpenGlWindow> getInstance();

protected:
    virtual void init() { }

    virtual void cleanup() { }

private:
    GlfwOpenGlWindowExample() { }

};

shared_ptr<GlfwOpenGlWindow> GlfwOpenGlWindowExample::getInstance() {
    static GlfwOpenGlWindow * instance = new GlfwOpenGlWindowExample();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

int main() {
    shared_ptr<GlfwOpenGlWindow> glfwDemo = GlfwOpenGlWindowExample::getInstance();
    glfwDemo->create(800, 600, "GlfwWindow Test");


    return 0;
}
