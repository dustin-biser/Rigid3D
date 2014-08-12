#version 400

in vec3 f_textureCoord;

uniform samplerCube skybox_texture;

out vec4 frag_colour;

void main () {
  frag_colour = texture (skybox_texture, f_textureCoord);
}