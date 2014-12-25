#include "SkyBoxDemo.hpp"

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
    shared_ptr<GlfwOpenGlWindow> SkyBoxDemo = SkyBoxDemo::getInstance();
    SkyBoxDemo->create(1024, 768, "Textured Cube Demo");

    return 0;
}
//---------------------------------------------------------------------------------------
SkyBoxDemo::SkyBoxDemo()
    : vao_cube(0),
      vbo_vertices(0),
      vbo_normals(0),
      vbo_textureCoords(0) {

}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> SkyBoxDemo::getInstance() {
    static GlfwOpenGlWindow * instance = new SkyBoxDemo();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::init() {
    texturedCube = Mesh("data/meshes/cube_uv_mapped.obj");

    camera.lookAt(vec3(0.0f), vec3(0.0f, 0.0f, -5.0f), vec3(0.0f, 1.0f, 0.0f));

    setupShaders();
    setupMatrices();
    setupUniformData();
    setupVertexData();
    setupSkyBoxVertexData();
    setupTextureData();
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::setupShaders() {
    cubeShader.attachVertexShader("data/shaders/PositionNormalTexture.vert");
    cubeShader.attachFragmentShader("data/shaders/ADS_Texture.frag");
    cubeShader.link();

    skyboxShader.attachVertexShader("data/shaders/SkyBox.vs");
    skyboxShader.attachFragmentShader("data/shaders/SkyBox.fs");
    skyboxShader.link();

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::setupMatrices(){
    modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, -10.0f));
    viewMatrix = camera.getViewMatrix();
    projectionMatrix = camera.getProjectionMatrix();
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::setupUniformData() {
    cubeShader.setUniform("material.emission", vec3(0.0f, 0.0f, 0.0f));
    cubeShader.setUniform("material.Ka", vec3(1.0f));
    cubeShader.setUniform("material.Kd", vec3(1.0f, 1.0f, 1.0f));
    cubeShader.setUniform("material.Ks", 0.2f);
    cubeShader.setUniform("material.shininessFactor", 0.1f);

    cubeShader.setUniform("lightSource.position", vec3(0.0f, 2.0f, 10.0f));
    cubeShader.setUniform("lightSource.rgbIntensity", vec3(1.0f, 1.0f, 1.0f));

    cubeShader.setUniform("ModelViewMatrix", viewMatrix * modelMatrix);
    cubeShader.setUniform("NormalMatrix", mat3(viewMatrix));
    cubeShader.setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);


    skyboxShader.setUniform("u_projectionMatrix", projectionMatrix);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::setupVertexData() {
    glGenVertexArrays(1, &vao_cube);
    glBindVertexArray(vao_cube);

    // Copy vertex position data to its respective GL buffer.
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, texturedCube.getNumVertexPositionBytes(),
                 texturedCube.getVertexPositionDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(cubeShader.getAttribLocation("v_Position"), 3, GL_FLOAT,
                          GL_FALSE, 0, 0);

    // Copy vertex normals to its respective GL buffer.
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, texturedCube.getNumVertexNormalBytes(),
                 texturedCube.getVertexNormalDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(cubeShader.getAttribLocation("v_Normal"), 3, GL_FLOAT, GL_FALSE,
                          0, 0);

    // Copy texture coordinates to its respective GL buffer.
    glGenBuffers(1, &vbo_textureCoords);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_textureCoords);
    glBufferData(GL_ARRAY_BUFFER, texturedCube.getNumTextureCoordBytes(),
                 texturedCube.getTextureCoordDataPtr(), GL_STATIC_DRAW);
    glVertexAttribPointer(cubeShader.getAttribLocation("v_TextureCoord"), 2, GL_FLOAT,
                          GL_FALSE, 0, 0);

    // Enable vertex attribute arrays.
    glEnableVertexAttribArray(cubeShader.getAttribLocation("v_Position"));
    glEnableVertexAttribArray(cubeShader.getAttribLocation("v_Normal"));
    glEnableVertexAttribArray(cubeShader.getAttribLocation("v_TextureCoord"));

    glBindVertexArray(0);
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::setupSkyBoxVertexData() {
    float skybox_vertices[] = {
        -10.0f,  10.0f, -10.0f,
        -10.0f, -10.0f, -10.0f,
        10.0f, -10.0f, -10.0f,
        10.0f, -10.0f, -10.0f,
        10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f, -10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

        10.0f, -10.0f, -10.0f,
        10.0f, -10.0f,  10.0f,
        10.0f,  10.0f,  10.0f,
        10.0f,  10.0f,  10.0f,
        10.0f,  10.0f, -10.0f,
        10.0f, -10.0f, -10.0f,

        -10.0f, -10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
        10.0f,  10.0f,  10.0f,
        10.0f,  10.0f,  10.0f,
        10.0f, -10.0f,  10.0f,
        -10.0f, -10.0f,  10.0f,

        -10.0f,  10.0f, -10.0f,
        10.0f,  10.0f, -10.0f,
        10.0f,  10.0f,  10.0f,
        10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f,  10.0f,
        -10.0f,  10.0f, -10.0f,

        -10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
        10.0f, -10.0f, -10.0f,
        10.0f, -10.0f, -10.0f,
        -10.0f, -10.0f,  10.0f,
        10.0f, -10.0f,  10.0f
    };

    glGenBuffers(1, &vbo_skybox_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_skybox_vertices);
    glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &skybox_vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_skybox);
    glBindVertexArray(vao_skybox);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_skybox_vertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void loadCubeMapTexture(GLuint texture, GLenum sideTarget, const char * fileName) {
    std::vector<unsigned char> png;
    std::vector<unsigned char> imageData; //the raw pixels
    unsigned width, height;

    //load and decode
    lodepng::load_file(png, fileName);
    unsigned error = lodepng::decode(imageData, width, height, png);
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    glBindTexture (GL_TEXTURE_CUBE_MAP, texture);

    // copy image data into 'target' side of cube map
    glTexImage2D(sideTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid *>(imageData.data()));

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::setupTextureData() {
    //-- Load Cube texture data
    {
        std::vector<unsigned char> png;
        std::vector<unsigned char> imageData; //the raw pixels
        unsigned width, height;

        //load and decode
        lodepng::load_file(png, "data/textures/uvgrid.png");
        unsigned error = lodepng::decode(imageData, width, height, png);

        //if there's an error, display it
        if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

        // Pass the image data to OpenGL.
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &cubeTexture);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid *>(imageData.data()));

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Set fragment shader's sampler2D to use texture unit 0.
        cubeShader.setUniform("texture2d", 0);
    }
    CHECK_GL_ERRORS;


    //-- Load Skybox cube map texture:
    {
        glActiveTexture(GL_TEXTURE1);
        glGenTextures(1, &skyboxTexture);

        const char * front =
            "data/textures/cube_maps/TropicalSunnyDay/TropicalSunnyDayFront512.png";

        const char * back =
            "data/textures/cube_maps/TropicalSunnyDay/TropicalSunnyDayBack512.png";

        const char * left =
            "data/textures/cube_maps/TropicalSunnyDay/TropicalSunnyDayLeft512.png";

        const char * right =
            "data/textures/cube_maps/TropicalSunnyDay/TropicalSunnyDayRight512.png";

        const char * up =
            "data/textures/cube_maps/TropicalSunnyDay/TropicalSunnyDayUp512.png";

        const char * down =
            "data/textures/cube_maps/TropicalSunnyDay/TropicalSunnyDayDown512.png";

        loadCubeMapTexture(skyboxTexture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
        loadCubeMapTexture(skyboxTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
        loadCubeMapTexture(skyboxTexture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
        loadCubeMapTexture(skyboxTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
        loadCubeMapTexture(skyboxTexture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, down);
        loadCubeMapTexture(skyboxTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, up);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        skyboxShader.setUniform("skybox_texture", 1);
    }
    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::setupGl(){
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
void SkyBoxDemo::draw() {
    glDepthMask(GL_FALSE);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glBindVertexArray(vao_skybox);
    skyboxShader.enable();
        glDrawArrays(GL_TRIANGLES, 0, 36);
    skyboxShader.disable();


    glDepthMask(GL_TRUE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    glBindVertexArray(vao_cube);
    cubeShader.enable();
        glDrawArrays(GL_TRIANGLES, 0, texturedCube.getNumVertexPositions());
    cubeShader.disable();


    glBindVertexArray(0);
}
//---------------------------------------------------------------------------------------
void SkyBoxDemo::logic() {
    modelMatrix = translate(mat4(), vec3(0.0f, 0.0f, -10.0f));
    viewMatrix = camera.getViewMatrix();
    projectionMatrix = camera.getProjectionMatrix();

    cubeShader.setUniform("ModelViewMatrix", viewMatrix * modelMatrix);
    cubeShader.setUniform("NormalMatrix", mat3(viewMatrix));
    cubeShader.setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);

    // Load only the rotational part of the camera's viewMatrix:
    mat4 rotMatrix = mat4(mat3(viewMatrix));
    skyboxShader.setUniform("u_viewMatrix", rotMatrix);
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::cleanup() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_textureCoords);
    glDeleteBuffers(1, &vbo_skybox_vertices);

    glDeleteBuffers(1, &vao_cube);
    glDeleteBuffers(1, &vao_skybox);

    glDeleteTextures(1, &cubeTexture);
    glDeleteTextures(1, &skyboxTexture);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void SkyBoxDemo::reloadShaderProgram() {

}
