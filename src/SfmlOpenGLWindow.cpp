#include <SfmlOpenGLWindow.hpp>

void SfmlOpenGLWindow::start() {
    start(800, 600);
}

void SfmlOpenGLWindow::start(int width, int height) {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2;
    settings.majorVersion = 4;
    settings.minorVersion = 2;

    // Create the window
    sf::Window window(sf::VideoMode(width, height), windowTitle, sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    centerWindow(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    setupGL();

    init();

    isRunning = true;
    while (isRunning) {
        logic();

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                // End the program.
                isRunning = false;
            }
            if (event.type == sf::Event::Closed) {
                // End the program.
                isRunning = false;

            } else if (event.type == sf::Event::Resized) {
                // Resize the OpenGL window.
                resize(event.size.width, event.size.height);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw();

        // End the current frame (internally swaps the front and back buffers)
        window.display();
    }

    cleanup();
}

void SfmlOpenGLWindow::centerWindow(sf::Window & window) {
    unsigned int x = sf::VideoMode::getDesktopMode().width;
    unsigned int y = sf::VideoMode::getDesktopMode().height;

    sf::Vector2u windowDimension = window.getSize();
    x = (x - windowDimension.x ) / 2;
    y = (y - windowDimension.y ) / 2;

    window.setPosition(sf::Vector2<int>(x,y));
}

void SfmlOpenGLWindow::setupGL() {
    // TODO - Uncomment the following once LoadMeshObj_Example is working

    // Render only the front face of geometry.
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CCW);

    // Setup depth testing
//    glEnable(GL_DEPTH_TEST);
//    glDepthMask(true);
//    glDepthFunc(GL_LEQUAL);
//    glDepthRange(0.0f, 1.0f);
//    glEnable(GL_DEPTH_CLAMP);

    glClearColor(0.3, 0.5, 0.7, 0.0);
}

void SfmlOpenGLWindow::close() {
    isRunning = false;
}
