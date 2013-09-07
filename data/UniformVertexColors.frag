#version 400

smooth in vec4 vertexColorOut;

out vec4 outputColor;

void main()
{
    outputColor = vertexColorOut;
}
