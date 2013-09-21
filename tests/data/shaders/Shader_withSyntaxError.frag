#version 330

smoth in vec4 passColor;

out vec4 outputColor

void main(
{
	float lerpValue = gl_FragCoord.y / 500.0f;
		
	outputColor = mix(passColor, passColor * 0.2, lerpValue)
}
