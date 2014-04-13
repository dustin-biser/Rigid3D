#include "RenderableFrustum.hpp"

#include <Rigid3D/Graphics/GlErrorCheck.hpp>

#include <OpenGL/gl3.h>

#include <glm/gtx/transform.hpp>

#include <vector>


namespace Rigid3D {

using glm::translate;
using std::vector;

//---------------------------------------------------------------------------------------
RenderableFrustum::RenderableFrustum()
    : vao(0), vbo_vertices(0) {

}

//---------------------------------------------------------------------------------------
RenderableFrustum::RenderableFrustum(const Frustum & frustum)
    : Frustum(frustum), vao(0), vbo_vertices(0) {

    setupData();
}

//---------------------------------------------------------------------------------------
RenderableFrustum::~RenderableFrustum() {
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vao);
}


//---------------------------------------------------------------------------------------
// To prevent temporary objects from deleting GL buffers within their destructors.
RenderableFrustum & RenderableFrustum::operator = (RenderableFrustum && rf) {
    Frustum::operator=(rf);
    vao = rf.vao;
    vbo_vertices = rf.vbo_vertices;

    rf.vao = 0;
    rf.vbo_vertices = 0;

    return *this;
}

//---------------------------------------------------------------------------------------
void RenderableFrustum::setupData() {
    float tanAngle = tan(fovy/2);

    float yNear = zNear * tanAngle;
    float xNear = aspectRatio * yNear;
    float yFar = zFar * tanAngle;
    float xFar = aspectRatio * yFar;

    vec3 nearLeftTop(-xNear, yNear, -1.0f * zNear);
    vec3 nearLeftBottom(-xNear, -yNear, -1.0f * zNear);
    vec3 nearRightBottom(xNear, -yNear, -1.0f * zNear);
    vec3 nearRightTop(xNear, yNear, -1.0f * zNear);

    vec3 farLeftTop(-xFar, yFar, -1.0f * zFar);
    vec3 farLeftBottom(-xFar, -yFar, -1.0f * zFar);
    vec3 farRightBottom(xFar, -yFar, -1.0f * zFar);
    vec3 farRightTop(xFar, yFar, -1.0f * zFar);

    vector<vec3> vertices = {
        nearLeftTop, nearLeftBottom, nearRightBottom, nearRightTop,     // front face
        farLeftTop, farLeftBottom, farRightBottom, farRightTop,         // far face
        nearLeftBottom, nearRightBottom, farRightBottom, farLeftBottom, // bottom face
        nearLeftTop, nearRightTop, farRightTop, farLeftTop,             // top face
        nearLeftBottom, farLeftBottom, farLeftTop, nearLeftTop,         // left face
        nearRightBottom, farRightBottom, farRightTop, nearRightTop      // right face
    };

    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    GLsizeiptr numBytes = vertices.size() * 3 * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, numBytes, const_cast<float *>(&((vertices.data())->x)), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//---------------------------------------------------------------------------------------
void RenderableFrustum::render(unsigned int vertexAttribIndex) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(vertexAttribIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexAttribIndex);

    int indicesPerFace = 4;
    int numFaces = 6;
    for(int i = 0; i < numFaces; i++) {
        glDrawArrays(GL_LINE_LOOP, i*indicesPerFace, indicesPerFace);
    }

    glBindVertexArray(0);
    glBindVertexArray(0);

    checkGLErrors(__FILE__, __LINE__);
}

//---------------------------------------------------------------------------------------
void RenderableFrustum::setColor(const vec4 & rgba) {
   this->rgba = rgba;
}

//---------------------------------------------------------------------------------------
void RenderableFrustum::setViewMatrix(const mat4 & viewMatrix) {
   this->viewMatrix = viewMatrix;
}

//---------------------------------------------------------------------------------------
void RenderableFrustum::setModelMatrix(const mat4 & modelMatrix) {
   this->modelMatrix = modelMatrix;
}

} // end namespace GlUtils.
