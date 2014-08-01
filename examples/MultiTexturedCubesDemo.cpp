#include "MultiTexturedCubesDemo.hpp"

#include <LoadPNG/lodepng.h>
using namespace lodepng;

#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
using namespace glm;

#include <iostream>
using namespace std;

//---------------------------------------------------------------------------------------
int main() {
    shared_ptr<GlfwOpenGlWindow> MultiTexturedCubesDemo = MultiTexturedCubesDemo::getInstance();
    MultiTexturedCubesDemo->create(1024, 768, "Multiple Textured Cubes");

    return 0;
}
//---------------------------------------------------------------------------------------
MultiTexturedCubesDemo::MultiTexturedCubesDemo()
    : vao(0),
      vbo_vertices(0),
      vbo_normals(0),
      vbo_textureCoords(0) {

}

//---------------------------------------------------------------------------------------
shared_ptr<GlfwOpenGlWindow> MultiTexturedCubesDemo::getInstance() {
    static GlfwOpenGlWindow * instance = new MultiTexturedCubesDemo();
    if (p_instance == nullptr) {
        p_instance = shared_ptr<GlfwOpenGlWindow>(instance);
    }

    return p_instance;
}

//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::init() {
    texturedCubeA = Mesh("data/meshes/cube_uv_mapped.obj");
    texturedCubeB = Mesh("data/meshes/cube_uv_mapped.obj");

    camera.lookAt(vec3(3,7,5), vec3(0.0f, 0.0f, -6.0f), vec3(0.0f, 1.0f, 0.0f));

    setupShaders();
    setupUniformData();
    setupVboData();
    setupVertexAttributeMapping();
    setupTextureData();
}

//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::setupShaders() {
    shader.loadFromFile("data/shaders/PositionNormalTexture.vert",
                        "data/shaders/ADS_Texture.frag");
}

//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::setupUniformData() {
    shader.setUniform("material.emission", vec3(0.0f, 0.0f, 0.0f));
    shader.setUniform("material.Ka", vec3(1.0f));
    shader.setUniform("material.Kd", vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("material.Ks", 0.2f);
    shader.setUniform("material.shininessFactor", 0.1f);

    shader.setUniform("lightSource.position", vec3(0.0f, 2.0f, 10.0f));
    shader.setUniform("lightSource.rgbIntensity", vec3(1.0f, 1.0f, 1.0f));

    CHECK_GL_ERRORS;
}
//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::transformVertexDataToWorldSpace() {

    //--Transform Vertex Positions.
    {
        size_t aBytes = texturedCubeA.getNumVertexPositionBytes();
        size_t aFloats = aBytes / sizeof(float32);

        modelMatrixA = translate(vec3(0, 0, -1));

        float32 * f = positionDataPtr.get();
        for (size_t i = 0; i < aFloats; i += 3) {
            vec4 position(f[i], f[i+1], f[i+2], 1);
            position = modelMatrixA * position;
            f[i] = position.x;
            f[i+1] = position.y;
            f[i+2] = position.z;
        }

        size_t bBytes = texturedCubeB.getNumVertexPositionBytes();
        size_t bFloats = bBytes / sizeof(float32);

        modelMatrixB = translate(vec3(0, 0, -6));

        for (size_t i = aFloats; i < aFloats + bFloats; i += 3) {
            float32 *f = positionDataPtr.get();
            vec4 position(f[i], f[i + 1], f[i + 2], 1);
            position = modelMatrixB * position;
            f[i] = position.x;
            f[i+1] = position.y;
            f[i+2] = position.z;
        }
    }
}

//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::setupVboData() {
    //-- Copy vertex position data into vbo_vertices.
    {
        size_t aBytes = texturedCubeA.getNumVertexPositionBytes();
        size_t bBytes = texturedCubeB.getNumVertexPositionBytes();
        size_t numBytes = aBytes + bBytes;

        size_t aFloats = aBytes / sizeof(float32);
        size_t bFloats = bBytes / sizeof(float32);
        size_t numFloats = aFloats + bFloats;

        positionDataPtr.reset(new float32[numFloats]);

        std::memcpy(positionDataPtr.get(),
                    texturedCubeA.getVertexPositionDataPtr(),
                    aBytes);

        std::memcpy(positionDataPtr.get() + aFloats,
                    texturedCubeB.getVertexPositionDataPtr(),
                    bBytes);

        transformVertexDataToWorldSpace();

        glGenBuffers(1, &vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, numBytes, positionDataPtr.get(), GL_STATIC_DRAW);
    }

    //-- Copy vertex normals into vbo_normals.
    {
        size_t aBytes = texturedCubeA.getNumVertexNormalBytes();
        size_t bBytes = texturedCubeB.getNumVertexNormalBytes();
        size_t numBytes = aBytes + bBytes;

        size_t aFloats = aBytes / sizeof(float32);
        size_t bFloats = bBytes / sizeof(float32);
        size_t numFloats = aFloats + bFloats;

        std::unique_ptr<float32[]> normalDataPtr(new float32[numFloats]);

        std::memcpy(normalDataPtr.get(),
                    texturedCubeA.getVertexNormalDataPtr(),
                    texturedCubeA.getNumVertexNormalBytes());

        std::memcpy(normalDataPtr.get() + aFloats,
                    texturedCubeB.getVertexNormalDataPtr(),
                    texturedCubeB.getNumVertexNormalBytes());

        glGenBuffers(1, &vbo_normals);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
        glBufferData(GL_ARRAY_BUFFER, numBytes, normalDataPtr.get(), GL_STATIC_DRAW);
    }

    //-- Copy texture coordinates into vbo_textureCoords.
    {
        size_t aBytes = texturedCubeA.getNumTextureCoordBytes();
        size_t bBytes = texturedCubeB.getNumTextureCoordBytes();
        size_t numBytes = aBytes + bBytes;

        size_t aFloats = aBytes / sizeof(float32);
        size_t bFloats = bBytes / sizeof(float32);
        size_t numFloats = aFloats + bFloats;
        std::unique_ptr<float32[]> textureCoordDataPtr(new float32[numFloats]);

        std::memcpy(textureCoordDataPtr.get(),
                    texturedCubeA.getTextureCoordDataPtr(),
                    texturedCubeA.getNumTextureCoordBytes());

        std::memcpy(textureCoordDataPtr.get() + aFloats,
                    texturedCubeB.getTextureCoordDataPtr(),
                    texturedCubeB.getNumTextureCoordBytes());

        glGenBuffers(1, &vbo_textureCoords);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_textureCoords);
        glBufferData(GL_ARRAY_BUFFER, numBytes, textureCoordDataPtr.get(), GL_STATIC_DRAW);
    }

    //-- Create indices for 1 textured cube.
    {
        glGenBuffers(1, &vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);

        size_t aElements = texturedCubeA.getNumVertexPositions();
        size_t bElements = texturedCubeB.getNumVertexPositions();
        numIndices =  aElements + bElements;

        // Add 3 in order to generate a degenerate triangle.
        numIndices += 3;

        std::unique_ptr<uint16[]> indexDataPtr(new uint16[numIndices]);

        for(int i = 0; i < aElements; ++i) {
            indexDataPtr[i] = i;
        }

        indexDataPtr[aElements] = aElements-1;
        indexDataPtr[aElements+1] = aElements-1;
        indexDataPtr[aElements+2] = aElements;

        for(int i = aElements; i < aElements + bElements; ++i) {
            indexDataPtr[i+3] = i;
        }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint16),
                     indexDataPtr.get(),  GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
}
//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::setupVertexAttributeMapping() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Enable vertex attribute arrays.
    glEnableVertexAttribArray(shader.getAttribLocation("v_Position"));
    glEnableVertexAttribArray(shader.getAttribLocation("v_Normal"));
    glEnableVertexAttribArray(shader.getAttribLocation("v_TextureCoord"));

    // Map vbo_vertices into vertex shader's "vertexPosition" location.
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(shader.getAttribLocation("v_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Map vbo_normals into vertex shader's "vertexNormal" location.
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glVertexAttribPointer(shader.getAttribLocation("v_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Map vbo_textureCoords into vertex shader's "vertexTextureCoord" location.
    glBindBuffer(GL_ARRAY_BUFFER, vbo_textureCoords);
    glVertexAttribPointer(shader.getAttribLocation("v_TextureCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::setupTextureData() {
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid *>(imageData.data()));

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Set fragment shader's sampler2D to use Texture Unit 0.
    shader.setUniform("texture2d", 0);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::setupGl(){
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
void MultiTexturedCubesDemo::draw() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);

    shader.enable();
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
    shader.disable();

    glBindVertexArray(0);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::logic() {
    viewMatrix = camera.getViewMatrix();
    projectionMatrix = camera.getProjectionMatrix();

    shader.setUniform("ModelViewMatrix", viewMatrix);
    shader.setUniform("NormalMatrix", mat3(viewMatrix));
    shader.setUniform("MVP", projectionMatrix * viewMatrix);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::cleanup() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &vbo_textureCoords);
    glDeleteBuffers(1, &vbo_indices);
    glDeleteBuffers(1, &vao);
    glDeleteTextures(1, &textureId);

    CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void MultiTexturedCubesDemo::reloadShaderProgram() {

}
