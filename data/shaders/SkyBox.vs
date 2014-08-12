#version 400

layout (location = 0) in vec3 v_position;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;

out vec3 f_textureCoord;

void main () {
  f_textureCoord = v_position;
  gl_Position = u_projectionMatrix * u_viewMatrix * vec4 (v_position, 1.0);
}