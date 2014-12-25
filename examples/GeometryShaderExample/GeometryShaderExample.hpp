#include "GlfwOpenGlWindow.hpp"

#include "Rigid3D.hpp"
using namespace Rigid3D;

/**
* @brief GeometryShaderExample
*
* @author Dustin Biser
*/

const uint32 position_attrib = 0;


class GeometryShaderExample : public GlfwOpenGlWindow {
public:

    static std::shared_ptr<GlfwOpenGlWindow> getInstance();

private:
    GeometryShaderExample(); // Singleton. Prevent direct construction.

    virtual void init();
    virtual void logic();
    virtual void draw();
//    virtual void keyInput(int key, int action, int mods);
//    virtual void cleanup();

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    ShaderProgram shaderProgram;

    void setupVboData();
    void setupShaders();

};
