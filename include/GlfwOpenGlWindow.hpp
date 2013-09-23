/**
 * @brief Abstract class for other classes to inherit that want access to an
 * GLFW OpenGL context and rendering window.
 *
 * @author Dustin Biser
 */

#ifndef GLFWOPENGLWINDOW_HPP_
#define GLFWOPENGLWINDOW_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Frustum.hpp>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <boost/utility.hpp>

using namespace std;
using namespace GlUtils;

using boost::noncopyable;
using glm::mat4;

///@brief Singleton
class GlfwOpenGlWindow : private noncopyable {
public:

    virtual ~GlfwOpenGlWindow();

    void setWindowTitle(string windowTitle) {
        this->windowTitle = windowTitle;
    }

    static shared_ptr<GlfwOpenGlWindow> getInstance();

    virtual void create(int width, int height, const string & windowTitle);

    virtual void close();

protected:
    GLFWwindow * window;
    string windowTitle;

    mat4 projectionMatrix;
    Frustum frustum;

    static shared_ptr<GlfwOpenGlWindow> p_instance;

    GlfwOpenGlWindow() : window(nullptr) { }

    static void error_callback(int error, const char* description);

    static void keyInputHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
    virtual void keyInput(int key, int scancode, int action, int mods);

    static void windowResizeHandler(GLFWwindow * window, int width, int height);
    virtual void resize(int width, int height);

    void centerWindow();

    void setupViewFrustum();

    void setupProjectionMatrix();

    virtual void init() { }

    virtual void logic() { }

    virtual void draw() { }

    virtual void cleanup() { }

    virtual void setupGl();

};

#endif /* GLFWOPENGLWINDOW_HPP_ */
