// LightFrustum.vert
#version 400

layout (location = 0) in vec3 vertexPosition;

out vec4 color;

uniform vec4 vertexColor;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void main() {
    vec4 position = ModelViewMatrix * vec4(vertexPosition, 1.0);
    color = vertexColor;
    gl_Position = ProjectionMatrix * position;
}