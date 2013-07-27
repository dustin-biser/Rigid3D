#version 330

layout (location = 0) in position;

uniforn  color;

smooth out vec4 passColor;

void main(
{
	gl_Position = position
	passColor = color;
}
