#version 330

layout (location = 0) in vec3 position;

uniform vec4 color;

smooth out vec4 passColor;

uniform mat4 cameraToClipMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 modelToWorldMatrix;

void main()
{
    mat4 modelToClipMatrix = cameraToClipMatrix * worldToCameraMatrix * modelToWorldMatrix; 
    gl_Position = modelToClipMatrix * vec4(position, 1.0);
    passColor = color;
}
