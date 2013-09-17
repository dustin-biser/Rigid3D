#version 400

in vec3 normal;

layout (location = 0) out vec4 fragColor;

void main() {
    fragColor = vec4(normalize(normal), 1.0);
}