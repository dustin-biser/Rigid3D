// DepthTexture.frag
#version 400

in vec2 textureCoord;

uniform sampler2D depthTexture;

layout (location = 0) out vec4 fragColor;

 // With depth comparison turned off, read from the depth value from the RED color channel.
void main() {
    vec4 depth = texture(depthTexture, textureCoord);
    float value = pow(depth.r, 10); // Make mono contrast more noticable.
    fragColor = vec4(value, value, value, 1.0);
}