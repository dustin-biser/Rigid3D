#include "TexturedCubeDemo.hpp"

#include <LoadPNG/lodepng.h>
using namespace lodepng;

#include "glm/gtx/transform.hpp"
using glm::translate;
using glm::inverse;
using glm::transpose;

#include <iostream>
using namespace std;

//---------------------------------------------------------------------------------------
int main() {
    shared_ptr<GlfwOpenGlWindow> texturedCubeDemo = TexturedCubeDemo::getInstance();
    texturedCubeDemo->create(1024, 768, "Textured Cube Demo");

    return 0;
}
//---------------------------------------------------------------------------------------
TexturedCubeDemo::TexturedCubeDemo()
    : vao(0),
      vbo_vertices(0),
      vbo_normals(0),
      vbo_textureCoords(0) {

}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> TexturedCubeDemo::getInstance() {
    static GlfwOpenGlWindow * instance = new TexturedCubeDemo();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::init() {
    texturedCube = Mesh("data/meshes/cube_uv_mapped.obj");

    camera.lookAt(vec3(0.0f), vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 1.0f, 0.0f));

    setupShaders();
    setupMatrices();
    setupUniformData();
    setupVertexData();
    setupTextureData();
}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::setupShaders() {
    shader.loadFromFile("data/shaders/PositionNormalTexture.vert",
                        "data/shaders/ADS_Texture.frag");

    Rigid3D::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::setupMatrices(){
    modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, -10.0f));
    viewMatrix = camera.getViewMatrix();
    projectionMatrix = camera.getProjectionMatrix();
}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::setupUniformData() {
    shader.setUniform("material.emission", vec3(0.0f, 0.0f, 0.0f));
    shader.setUniform("material.Ka", vec3(1.0f));
    shader.setUniform("material.Kd", vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("material.Ks", 0.2f);
    shader.setUniform("material.shininessFactor", 0.1f);

    shader.setUniform("lightSource.position", vec3(0.0f, 2.0f, 10.0f));
    shader.setUniform("lightSource.rgbIntensity", vec3(1.0f, 1.0f, 1.0f));

    shader.setUniform("ModelViewMatrix", viewMatrix * modelMatrix);
    shader.setUniform("NormalMatrix", mat3(viewMatrix));
    shader.setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);

    Rigid3D::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::setupVertexData() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Copy vertex position data to its respective GL buffer.
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, texturedCube.getNumVertexPositionBytes(),
            texturedCube.getVertexPositionDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shader.getAttribLocation("v_Position"), 3, GL_FLOAT,
                          GL_FALSE, 0, 0);

    // Copy vertex normals to its respective GL buffer.
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, texturedCube.getNumVertexNormalBytes(),
            texturedCube.getVertexNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shader.getAttribLocation("v_Normal"), 3, GL_FLOAT, GL_FALSE,
                          0, 0);

    // Copy texture coordinates to its respective GL buffer.
    glGenBuffers(1, &vbo_textureCoords);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_textureCoords);
    glBufferData(GL_ARRAY_BUFFER, texturedCube.getNumTextureCoordBytes(),
            texturedCube.getTextureCoordDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(shader.getAttribLocation("v_TextureCoord"), 2, GL_FLOAT,
                          GL_FALSE, 0, 0);

    // Enable vertex attribute arrays.
    glEnableVertexAttribArray(shader.getAttribLocation("v_Position"));
    glEnableVertexAttribArray(shader.getAttribLocation("v_Normal"));
    glEnableVertexAttribArray(shader.getAttribLocation("v_TextureCoord"));

    glBindVertexArray(0);
}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::setupTextureData() {
    std::vector<unsigned char> png;
    std::vector<unsigned char> imageData; //the raw pixels
    unsigned width, height;

    //load and decode
    lodepng::load_file(png, "data/textures/uvgrid.png");
    unsigned error = lodepng::decode(imageData, width, height, png);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

    // Pass the image data to OpenGL.
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid *>(imageData.data()));

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Set fragment shader's sampler2D to use texture unit 0.
    shader.setUniform("texture2d", 0);

    Rigid3D::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::setupGl(){
    // Render only the front face of geometry.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Setup depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_CLAMP);


    glClearDepth(1.0f);
    glClearColor(0.2, 0.2, 0.2, 1.0);
}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::draw() {
    glBindVertexArray(vao);
    shader.enable();
        glDrawArrays(GL_TRIANGLES, 0, texturedCube.getNumVertexPositions());
    shader.disable();
    glBindVertexArray(0);
}
//---------------------------------------------------------------------------------------
void TexturedCubeDemo::logic() {
    modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, -10.0f));
    viewMatrix = camera.getViewMatrix();
    projectionMatrix = camera.getProjectionMatrix();

    shader.setUniform("ModelViewMatrix", viewMatrix * modelMatrix);
    shader.setUniform("NormalMatrix", mat3(viewMatrix));
    shader.setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);

}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::cleanup() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_textureCoords);
    glDeleteBuffers(1, &vao);
    glDeleteTextures(1, &textureId);

    Rigid3D::checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void TexturedCubeDemo::reloadShaderProgram() {

}
