// PositionNormalTexture.vert
#version 400

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TextureCoord;

out vec3 position;
out vec3 normal;
out vec2 textureCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main() {
    textureCoord = v_TextureCoord;
    normal = normalize(NormalMatrix * v_Normal);
    position = vec3( ModelViewMatrix * vec4(v_Position, 1.0) );
    gl_Position = MVP * vec4(v_Position, 1.0);
}
