#ifndef LOADMESHOBJ_EXAMPLE_HPP_
#define LOADMESHOBJ_EXAMPLE_HPP_

#include <GL/glew.h>
#include <GL/gl.h>
#include <SFML/Graphics/Shader.hpp>
#include <Mesh.hpp>
#include <glm/gtc/reciprocal.hpp>
#include <vector>
#include <MathUtils.hpp>
#include <Frustum.hpp>
#include <SfmlOpenGLWindow.hpp>

using GlUtils::Mesh;
using GlUtils::Frustum;
using sf::Shader;
using glm::mat4;

class LoadMeshObj_Example : public SfmlOpenGLWindow {

public:
    virtual void setupGL();

    virtual void init();

    virtual void logic();

    virtual void draw();

    virtual void resize(int width, int height);

    virtual void cleanup();


protected:
    Shader shader;
    Mesh mesh;
    Frustum frustum;
    mat4 worldToCameraMatrix;
    mat4 cameraToClipMatrix;

    GLuint vertexBufferObject;
    GLuint vao;

    void InitializeVertexBuffer();
    void setupShaders();
    void setupMatrices();
    void setupVertexBuffer();
    void setupVertexArrayObject();

};

#endif /* LOADMESHOBJ_EXAMPLE_HPP_ */
