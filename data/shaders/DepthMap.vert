// DepthMath.vert
#version 400

layout (location = 0) in vec3 vertexPosition;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    // Transform vertex position to eye coordinate space.
    vec4 position = ViewMatrix * (ModelMatrix * vec4(vertexPosition, 1.0));

    // Transform position to homogeneous clip space.
    gl_Position = ProjectionMatrix * position;
}
