#version 330

// Linker error caused by missing main() method.

smooth in vec4 passColor;

out vec4 outputColor;

float lerpValue = gl_FragCoord.y / 500.0f;
	
outputColor = mix(passColor, passColor * 0.2, lerpValue);
