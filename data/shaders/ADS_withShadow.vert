// ADS_withShadow.vert
#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 normal;
out vec3 position;
out vec4 shadowCoord; // Coordinate to be used for shadow map lookup.

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP; // model view projection matrix.
uniform mat4 ShadowMatrix; // transforms points from model space to shadow map space.

void main() {
    vec4 posVec4 = vec4(vertexPosition, 1.0);

    position = (ModelViewMatrix * posVec4).xyz;
    normal = normalize(NormalMatrix * vertexNormal);
    
    shadowCoord = ShadowMatrix * posVec4;
    gl_Position = MVP * posVec4;
}