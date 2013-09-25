#version 330

layout (location = 0) in vec4 position;

uniform bool boolUniform;
uniform int intUniform;
uniform uint uintUniform;
uniform float floatUniform;
uniform vec2 vec2Uniform;
uniform vec3 vec3Uniform;
uniform vec4 vec4Uniform;
uniform mat2 mat2Uniform;
uniform mat3 mat3Uniform;
uniform mat4 mat4Uniform;

smooth out vec4 passColor;

void main()
{
    if (boolUniform) {
		passColor = mat4(mat2Uniform) * mat4(mat3Uniform) * mat4Uniform * vec4Uniform;
    }
    else {
        passColor = vec4(floatUniform, floatUniform, floatUniform, float(intUniform)) +
            vec4(vec2Uniform.x, vec2Uniform.y, 1.0, 1.0) + vec4(vec3Uniform.x, vec3Uniform.y, vec3Uniform.z, 1.0); 
    }

	gl_Position = position + vec4(uintUniform);
}
