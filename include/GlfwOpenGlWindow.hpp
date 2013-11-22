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

#include "Camera.hpp"
using GlUtils::Camera;

#include "CameraController.hpp"
using GlUtils::CameraController;

#include <string>
#include <memory>
using std::string;
using std::shared_ptr;

#include <glm/glm.hpp>

#include <boost/utility.hpp>
using boost::noncopyable;

/// Singleton
class GlfwOpenGlWindow : private noncopyable {
public:
    virtual ~GlfwOpenGlWindow();

    static shared_ptr<GlfwOpenGlWindow> getInstance();

    void create(int width, int height, const string & windowTitle);

protected:
    GLFWwindow * window;
    GLFWwindow * prevWindow;
    GLFWmonitor * monitor;
    string windowTitle;
    int windowWidth;
    int windowHeight;

    bool paused;
    bool fullScreen;
    bool destroyPrevWindow;

    Camera camera;
    CameraController cameraController;

    static shared_ptr<GlfwOpenGlWindow> p_instance;

    GlfwOpenGlWindow();

    static void error_callback(int error, const char* description);

    static void keyInputCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    void keyInputBase(int key, int action, int mods);
    virtual void keyInput(int key, int action, int mods);

    static void windowResizeCallBack(GLFWwindow * window, int width, int height);
    virtual void resize(int width, int height);

    static void mouseScrollCallBack(GLFWwindow * window, double xOffSet, double yOffSet);
    void mouseScrollBase(double xOffSet, double yOffSet);
    virtual void mouseScroll(double xOffSet, double yOffSet);

    static void mouseButtonCallBack(GLFWwindow * window, int button , int actions, int mods);
    void mouseButtonInputBase(int button , int actions, int mods);
    virtual void mouseButtonInput(int button , int actions, int mods);

    static void cursorPositionCallBack(GLFWwindow * window, double xPos, double yPos);
    void cursorPositionBase(double xPos, double yPos);
    virtual void cursorPosition(double xPos, double yPos);

    static void cursorEnterCallBack(GLFWwindow * window, int entered);
    void cursorEnterBase(int entered);
    virtual void cursorEnter(int entered);

    void registerGlfwCallBacks();

    void destroyPrevWindowCheck();

    void centerWindow();

    void initNewOpenGlContext();

    void switchToFullScreen();

    void switchToWindowedMode();

    void setupCamera();

    virtual void init() { }
    virtual void setupGl();
    virtual void reloadShaderProgram() { }
    virtual void logic() { }
    virtual void draw() { }
    virtual void cleanup() { }

};

#endif /* GLFWOPENGLWINDOW_HPP_ */
