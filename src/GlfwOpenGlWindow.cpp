#include <GlfwOpenGlWindow.hpp>
#include <GlfwException.hpp>
#include <GlErrorCheck.hpp>
#include <sstream>

using namespace std;

shared_ptr<GlfwOpenGlWindow> GlfwOpenGlWindow::p_instance = nullptr;

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::error_callback(int error, const char* description) {
    throw GlfwException(description);
}

//----------------------------------------------------------------------------------------
/**
 * @brief Window resize call back function to be registered with GLFW.
 */
void GlfwOpenGlWindow::windowResizeHandler(GLFWwindow * window, int width, int height) {
    getInstance()->resize(width, height);
}

//----------------------------------------------------------------------------------------
/**
 * @brief Window resize call back function to be overridden by derived classes.
 */
void GlfwOpenGlWindow::resize(int widith, int height) {

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
void GlfwOpenGlWindow::create(int width, int height, const string & windowTitle) {
    glfwSetErrorCallback(error_callback);

    if (glfwInit() == GL_FALSE) {
        throw GlfwException("Call to glfwInit() failed.");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

    // Create Opengl Window
    window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw GlfwException("Call to glfwCreateWindow failed.");
    }

    centerWindow();
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyInputHandler);
    glfwSetWindowSizeCallback(window, windowResizeHandler);

    // Initialize OpenGL extensions with GLEW
    glewExperimental = GL_TRUE;
    GLenum errorCode = glewInit();
    if (errorCode != GLEW_OK) {
        stringstream errorMessage;
        errorMessage << "Failed to initialize GLEW -- " << gluErrorString(errorCode);
        throw GlfwException(errorMessage.str());
    }
    // Clear error buffer.  Specifically due to glewInit() causing a
    // GLError(invalid enumerant), which is safe to ignore.
    // http://www.opengl.org/wiki/OpenGL_Loading_Library
    while(glGetError() != GL_NO_ERROR);

    setupGl();
    init();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        logic();
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
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
/**
 * @brief Keyboard input call back function to be registered with GLFW.
 */
void GlfwOpenGlWindow::keyInputHandler(GLFWwindow* window, int key, int scancode,
        int action, int mods) {
    getInstance()->keyInput(key, scancode, action, mods);
}

//----------------------------------------------------------------------------------------
/**
 * @brief Keyboard input call back function to be overridden by derived classes.
 */
void GlfwOpenGlWindow::keyInput(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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

    glClearColor(0.3, 0.5, 0.7, 1.0);
}

//----------------------------------------------------------------------------------------
/**
 * @brief Closes the OpenGL window and terminates the main loop.
 */
void GlfwOpenGlWindow::close() {
    glfwSetWindowShouldClose(window, GL_TRUE);
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::draw() {
    // To be overridden by derived class
}
