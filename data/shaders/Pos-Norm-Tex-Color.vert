// Pos-Norm-Tex-Color.vert
#version 420
#extension GL_ARB_explicit_uniform_location : enable

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTextureCoord;
layout (location = 3) in vec3 vertexColor;

out vec3 position;
out vec3 normal;
out vec2 textureCoord;
out vec3 color;

layout (location = 0) uniform mat4 ModelViewMatrix;
layout (location = 1) uniform mat3 NormalMatrix;
layout (location = 2) uniform mat4 ProjectionMatrix;

void main()
{
    // Transform vertex position and normal to eye coordinate space.
    normal = normalize(NormalMatrix * vertexNormal);
    position = vec3( ModelViewMatrix * vec4(vertexPosition, 1.0) );
    
    color = vertexColor;

    // Transform position to normalized device coordinate space.
    gl_Position = ProjectionMatrix * vec4(position, 1.0);
}