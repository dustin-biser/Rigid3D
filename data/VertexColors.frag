#version 330

smooth in vec4 theColor;

out vec4 outputColor;

void main()
{
	float lerpValue = gl_FragCoord.y / 500.0f;
		
	outputColor = mix(theColor, theColor * 0.2, lerpValue);
}
