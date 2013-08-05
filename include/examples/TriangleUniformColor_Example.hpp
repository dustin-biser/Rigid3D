#ifndef TRIANGLE_UNIFORM_COLOR_EXAMPLE_HPP_
#define TRIANGLE_UNIFORM_COLOR_EXAMPLE_HPP_

#include <SfmlOpenGLWindow.hpp>
#include <ShaderProgram.hpp>
#include <vector>

using namespace GlUtils;
using namespace std;

class TriangleUniformColor_Example : public SfmlOpenGLWindow {
public:
    virtual void init();

    virtual void logic();

    virtual void draw();

    virtual void resize(int width, int height);

    virtual void cleanup();

    ~TriangleUniformColor_Example() { }

protected:
    GLuint vertexBufferObject;
    GLuint vao;
    GLuint vbo_indices;
    ShaderProgram shaderProgram;

    const float vertexData[12] = {
        -0.5f, -0.366f, 0.0f, 1.0f,
        0.5f, -0.366f, 0.0f, 1.0f,
        0.0f,    0.5f, 0.0f, 1.0f
    };

    vector<unsigned short> indices = { 0, 1, 2 };

    void initShaders();

    void initGLData();
};


#endif /* TRIANGLE_UNIFORM_COLOR_EXAMPLE_HPP_ */
