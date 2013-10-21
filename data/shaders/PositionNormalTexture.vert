// PositionNormalTexture.vert
#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTextureCoord;

out vec3 position;
out vec3 normal;
out vec2 textureCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main() {
    textureCoord = vertexTextureCoord;
    normal = normalize(NormalMatrix * vertexNormal); 
    position = vec3( ModelViewMatrix * vec4(vertexPosition, 1.0) );
    gl_Position = MVP * vec4(vertexPosition, 1.0);
}
