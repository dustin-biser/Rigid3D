#include <GlfwOpenGlWindow.hpp>
#include <GlfwException.hpp>
#include <GlErrorCheck.hpp>
#include <MathUtils/MathUtils.hpp>
#include <sstream>

using MathUtils::degreesToRadians;
using MathUtils::cotangent;


shared_ptr<GlfwOpenGlWindow> GlfwOpenGlWindow::p_instance = nullptr;

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
    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);


    // Create Opengl Window
    window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw GlfwException("Call to glfwCreateWindow failed.");
    }

    centerWindow();
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyInputCallBack);
    glfwSetWindowSizeCallback(window, windowResizeCallBack);

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
    setupCamera();
    setupProjectionMatrix();
    init();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        logic();
        draw();
        glfwSwapBuffers(window);
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
void GlfwOpenGlWindow::keyInputCallBack(GLFWwindow* window, int key, int scancode,
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
/**
 * @brief Mouse scroll call back function to be registered with GLFW.
 */
void GlfwOpenGlWindow::mouseScrollCallBack(GLFWwindow * window, double xOffSet, double yOffSet) {
    getInstance()->mouseScroll(xOffSet, yOffSet);
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
}

//----------------------------------------------------------------------------------------
void GlfwOpenGlWindow::setupProjectionMatrix() {
    projectionMatrix = camera.getProjectionMatrix();
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
