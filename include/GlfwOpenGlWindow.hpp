/**
 * @brief Abstract class for other classes to inherit from that want access to an
 * GLFW OpenGL context window for rendering.
 *
 * @author Dustin Biser
 */

#ifndef GLFWOPENGLWINDOW_HPP_
#define GLFWOPENGLWINDOW_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Camera.hpp>
using GlUtils::Camera;

#include <string>
#include <memory>
using std::string;
using std::shared_ptr;

#include <glm/glm.hpp>

#include <boost/utility.hpp>
using boost::noncopyable;

///@brief Singleton
class GlfwOpenGlWindow : private noncopyable {
public:
    virtual ~GlfwOpenGlWindow();

    static shared_ptr<GlfwOpenGlWindow> getInstance();

    void create(int width, int height, const string & windowTitle);

    void setWindowTitle(string windowTitle);

    void close();

protected:
    GLFWwindow * window;
    string windowTitle;
    int windowWidth;
    int windowHeight;
    Camera camera;
    static shared_ptr<GlfwOpenGlWindow> p_instance;
    bool paused;

    GlfwOpenGlWindow();

    static void error_callback(int error, const char* description);

    static void keyInputCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    virtual void keyInput(int key, int scancode, int action, int mods);

    static void windowResizeCallBack(GLFWwindow * window, int width, int height);
    virtual void resize(int width, int height);

    static void mouseScrollCallBack(GLFWwindow * window, double xOffSet, double yOffSet);
    virtual void mouseScroll(double xOffSet, double yOffSet) { }

    void centerWindow();

    void setupCamera();

    virtual void init() { }
    virtual void setupGl();
    virtual void reloadShaderProgram() { }
    virtual void logic() { }
    virtual void draw() { }
    virtual void cleanup() { }

};

#endif /* GLFWOPENGLWINDOW_HPP_ */
