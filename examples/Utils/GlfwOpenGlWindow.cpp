#include "GlfwOpenGlWindow.hpp"
using Rigid3D::Camera;
using Rigid3D::CameraController;

#include <Rigid3D/Graphics/GlfwException.hpp>
#include <Rigid3D/Graphics/GlErrorCheck.hpp>

#include <Rigid3D/Math/Trigonometry.hpp>
using Rigid3D::cotangent;
using Rigid3D::degreesToRadians;

#include <sstream>
#include <iostream>
using std::cout;
using std::endl;
using std::stringstream;
using std::shared_ptr;
using std::string;

shared_ptr<GlfwOpenGlWindow> GlfwOpenGlWindow::p_instance = nullptr;

//----------------------------------------------------------------------------------------
GlfwOpenGlWindow::GlfwOpenGlWindow()
 : window(nullptr),
   windowTitle(),
   windowWidth(0),
   windowHeight(0),
   paused(false),
   fullScreen(false),
   destroyPrevWindow(false),
   camera(),
   cameraController() {

}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::error_callback(int error, const char* description) {
    throw GlfwException(description);
}

//----------------------------------------------------------------------------------------
/**
 * @brief Window resize call back function to be registered with GLFW.
 */
void GlfwOpenGlWindow::windowResizeCallBack(GLFWwindow * window, int width, int height) {
    getInstance()->resize(width, height);
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::resize(int width, int height) {
    float aspectRatio = ((float) width) / height;
    float frustumYScale = cotangent(degreesToRadians(camera.getFieldOfViewY() / 2));

    float frustumXScale = frustumYScale;
    glm::mat4 projectionMatrix = camera.getProjectionMatrix();

    if (width > height) {
        // Shrink the x scale in eye-coordinate space, so that when geometry is
        // projected to ndc-space, it is widened out to become square.
        projectionMatrix[0][0] = frustumXScale / aspectRatio;
        projectionMatrix[1][1] = frustumYScale;
    }
    else {
        // Shrink the y scale in eye-coordinate space, so that when geometry is
        // projected to ndc-space, it is widened out to become square.
        projectionMatrix[0][0] = frustumXScale;
        projectionMatrix[1][1] = frustumYScale * aspectRatio;
    }

    camera.setProjectionMatrix(projectionMatrix);

    // Use entire window for rendering.
    glViewport(0, 0, width, height);
}

//----------------------------------------------------------------------------------------
/**
 * @return the instance of this GlfwOpenGlWindow
 */
shared_ptr<GlfwOpenGlWindow> GlfwOpenGlWindow::getInstance() {
    static GlfwOpenGlWindow * instance = new GlfwOpenGlWindow();
    if (p_instance == nullptr) {
        // Pass ownership of instance to shared_ptr.
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }
    return p_instance;
}

//----------------------------------------------------------------------------------------
static void printGLInfo() {
    const GLubyte * renderer = glGetString( GL_RENDERER );
    const GLubyte * vendor = glGetString( GL_VENDOR );
    const GLubyte * version = glGetString( GL_VERSION );
    const GLubyte * glsl_version = glGetString( GL_SHADING_LANGUAGE_VERSION );
    
    GLint majorVersion, minorVersion;
    
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    
    stringstream message;
    
    if (vendor)
        message << "GL Vendor          : " << vendor << endl;
   
    if (renderer)
        message << "GL Renderer        : " << renderer << endl;
    
    if (version)
        message << "GL Version         : " << version << endl;
    
    message << "GL Version         : " << majorVersion << "." << minorVersion << endl
            << "GLSL Version       : " << glsl_version << endl;
    
    cout << message.str();
    
    cout << "Supported Extensions: " << endl;
    const GLubyte * extension;
    GLint nExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
    
    for (int i = 0; i < nExtensions; ++i) {
        extension = glGetStringi(GL_EXTENSIONS, i);
        if (extension) {
            cout << extension << endl;
        }
    }
    
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::create(int width, int height, const string & windowTitle) {
    windowWidth = width;
    windowHeight = height;
    glfwSetErrorCallback(error_callback);

    if (glfwInit() == GL_FALSE) {
        throw GlfwException("Call to glfwInit() failed.");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    monitor = glfwGetPrimaryMonitor();
    if (monitor == NULL) {
        glfwTerminate();
        throw GlfwException("Error retrieving primary monitor.");
    }

    // Create Opengl Window
    window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw GlfwException("Call to glfwCreateWindow failed.");
    }
    

    centerWindow();
    glfwMakeContextCurrent(window);
    
#ifdef DEBUG
    printGLInfo();
#endif

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    registerGlfwCallBacks();

    // Clear error buffer.
    while(glGetError() != GL_NO_ERROR);

    setupGl();
    setupCamera();
    init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (!paused) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cameraController.updateCamera();
            logic();
            draw();
            glfwSwapBuffers(window);
        }
        destroyPrevWindowCheck();
    }

    cleanup();
    glfwDestroyWindow(window);
}

//----------------------------------------------------------------------------------------
GlfwOpenGlWindow::~GlfwOpenGlWindow() {
    // Free all GLFW resources.
    glfwTerminate();
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::destroyPrevWindowCheck() {
    if (destroyPrevWindow) {
        glfwDestroyWindow(prevWindow);
        prevWindow = 0;
        destroyPrevWindow = false;
    }
}

//----------------------------------------------------------------------------------------
/**
 * Register all callbacks with GLFW library for the currently active rendering window.
 */
void GlfwOpenGlWindow::registerGlfwCallBacks() {
    glfwSetKeyCallback(window, keyInputCallBack);
    glfwSetWindowSizeCallback(window, windowResizeCallBack);
    glfwSetScrollCallback(window, mouseScrollCallBack);
    glfwSetMouseButtonCallback(window, mouseButtonCallBack);
    glfwSetCursorPosCallback(window, cursorPositionCallBack);
    glfwSetCursorEnterCallback(window, cursorEnterCallBack);
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::initNewOpenGlContext() {
    cleanup();
    registerGlfwCallBacks();
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setupGl();
    init();
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::switchToFullScreen() {
    prevWindow = window;
    destroyPrevWindow = true;

    const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);

    // Create Opengl Window
    window = glfwCreateWindow(videoMode->width, videoMode->height, windowTitle.c_str(), monitor, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw GlfwException("Call to glfwCreateWindow failed.");
    }
    resize(videoMode->width, videoMode->height);
    cameraController.reset();

    initNewOpenGlContext();
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::switchToWindowedMode() {
    prevWindow = window;
    destroyPrevWindow = true;

    // Create Opengl Window
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw GlfwException("Call to glfwCreateWindow failed.");
    }
    resize(windowWidth, windowHeight);
    centerWindow();
    cameraController.reset();

    initNewOpenGlContext();
}

//----------------------------------------------------------------------------------------
/**
 * @brief Keyboard input call back function to be registered with GLFW.
 */
void GlfwOpenGlWindow::keyInputCallBack(GLFWwindow* window, int key, int scancode,
        int action, int mods) {

    getInstance()->keyInputBase(key, action, mods);
    getInstance()->keyInput(key, action, mods);
}

//----------------------------------------------------------------------------------------
/**
 * Key input processing for base class. This method is always ran before callbacks
 * are issued for the overridden \c keyInput() method of any derived classes.
 *
 * @param key
 * @param action
 * @param mods
 */
void GlfwOpenGlWindow::keyInputBase(int key, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        } else if (key == GLFW_KEY_F11) {
            if (fullScreen == false) {
                switchToFullScreen();
            } else {
                switchToWindowedMode();
            }
            fullScreen = !fullScreen;
        } else if (key == GLFW_KEY_F10) {
            paused = !paused;
        } else if (key == GLFW_KEY_F8) {
            reloadShaderProgram();
        }
    }

    if (key == GLFW_KEY_LEFT_SHIFT) {
        if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    cameraController.keyInput(key, action, mods);

}

//----------------------------------------------------------------------------------------
/**
 * Keyboard input call back function to be overridden by derived classes.
 */
void GlfwOpenGlWindow::keyInput(int key, int action, int mods) {

}

//----------------------------------------------------------------------------------------
/**
 * brief Mouse scroll call back function to be registered with GLFW.
 */
void GlfwOpenGlWindow::mouseScrollCallBack(GLFWwindow * window, double xOffSet, double yOffSet) {
    getInstance()->mouseScrollBase(xOffSet, yOffSet);
    getInstance()->mouseScroll(xOffSet, yOffSet);
}

//----------------------------------------------------------------------------------------
/**
 * Mouse scroll processing for base class.  This method is always ran before callbacks
 * are issued for the overridden 'mouseScroll()' method of any derived class.
 */
void GlfwOpenGlWindow::mouseScrollBase(double xOffSet, double yOffSet) {
    cameraController.mouseScroll(xOffSet, yOffSet);
}

//----------------------------------------------------------------------------------------
/**
 * Mouse scroll call back function to be overridden by derived classes.
 */
void GlfwOpenGlWindow::mouseScroll(double xOffSet, double yOffSet) {

}

//----------------------------------------------------------------------------------------
/**
 * Mouse button call back function to be registered with GLFW.
 */
void GlfwOpenGlWindow::mouseButtonCallBack(GLFWwindow * window, int button , int actions, int mods) {
    getInstance()->mouseButtonInputBase(button, actions, mods);
    getInstance()->mouseButtonInput(button, actions, mods);
}

//----------------------------------------------------------------------------------------
/**
 * Mouse button input processing for base class. This method is always ran before callbacks
 * are issued for the overridden \c mouseButtonInput() method of any derived classes.
 *
 * @param button
 * @param actions
 * @param mods
 */
void GlfwOpenGlWindow::mouseButtonInputBase(int button , int actions, int mods) {
    cameraController.mouseButton(button, actions, mods);
}

//----------------------------------------------------------------------------------------
/**
 * Mouse button input callback function to be overridden by derived classes.
 */
void GlfwOpenGlWindow::mouseButtonInput(int button , int actions, int mods) {

}

//----------------------------------------------------------------------------------------
/**
 *  Cursor position callback to be registered with GLFW.
 */
void GlfwOpenGlWindow::cursorPositionCallBack(GLFWwindow * window, double xPos, double yPos) {
    getInstance()->cursorPositionBase(xPos, yPos);
    getInstance()->cursorPosition(xPos, yPos);
}

//----------------------------------------------------------------------------------------
/**
 * Cursor position input processing for base class.  This method is always ran before callbacks
 * are issued for the overridden \c cursorInput() method of any derived classes.
 *
 * @param xPos
 * @param yPos
 */
void GlfwOpenGlWindow::cursorPositionBase(double xPos, double yPos) {
    cameraController.cursorPosition(xPos, yPos);
}

//----------------------------------------------------------------------------------------
/**
 * Cursor position callback function to be overridden by derived classes.
 *
 * @param xPos - The new x-coordinate, in screen coordinates, of the cursor.
 * @param yPos - the new y-coordinate, in screen coordinates, of the cursor.
 */
void GlfwOpenGlWindow::cursorPosition(double xPos, double yPos) {

}

//----------------------------------------------------------------------------------------
/**
 * Callback function for when cursor enters/exits rendering window.  To be
 * registered with GLFW.
 */
void GlfwOpenGlWindow::cursorEnterCallBack(GLFWwindow * window, int entered) {
    getInstance()->cursorEnterBase(entered);
    getInstance()->cursorEnter(entered);
}

//----------------------------------------------------------------------------------------
/**
 * Cursor enter/exit window callback for base class. This method is always ran
 * before callbacks are issued for the overridden \c cursorEnter() method of any
 * derived classes.
 *
 * @param entered
 */
void GlfwOpenGlWindow::cursorEnterBase(int entered) {

}

//----------------------------------------------------------------------------------------
/**
 * Callback function for when cursor enters/exits rendering window.  To be
 * overridden by derived classes.
 *
 * @param entered - GL_TRUE if the cursor entered the window's client area, or
 *                  GL_FALSE if it left it.
 */
void GlfwOpenGlWindow::cursorEnter(int entered) {
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::centerWindow() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (monitor == NULL) {
        return;
    }

    int x, y;
    const GLFWvidmode *videoMode = glfwGetVideoMode(monitor);
    x = videoMode->width;
    y = videoMode->height;

    x = (x - windowWidth) / 2;
    y = (y - windowHeight) / 2;

    glfwSetWindowPos(window, x, y);
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::setupCamera() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    float aspectRatio = (float) windowWidth / (float) windowHeight;
    camera = Camera(45.0f, aspectRatio, 1.0f, 100.0f);

    cameraController.registerCamera(&camera);
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::setupGl() {
    // Render only the front face of geometry.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Setup depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_DEPTH_CLAMP);

    glClearDepth(1.0f);
    glClearColor(0.3, 0.5, 0.7, 1.0);
}
