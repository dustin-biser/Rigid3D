#version 330

layout (location = 0) in vec4 position;

uniform  vec4 color;

smooth out vec4 passColor;

void main()
{
	gl_Position = position;
	passColor = color;
}
