#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 vertexColorOut;

uniform vec3 lightPositionEC; // Light position in eye coordinates.
uniform vec3 Kd;  // Coefficient of diffuse reflectivity for RGB spectrum.
uniform vec3 Ld;  // Light source RGB color intensity vector.

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

void main() {
    // Transformed normal given in eye coordinates.
    vec3 normalEC = normalize(NormalMatrix * vertexNormal);
    
    // Transformed vertex position given in eye coordinates.
    vec4 vertexPositionEC = ModelViewMatrix * vec4(vertexPosition, 1.0f);
    
    // Light source vector
    vec3 s = normalize(vec3( vec4(lightPositionEC, 1.0f) - vertexPositionEC));
    
    // Diffuse shading equation
    vertexColorOut = Ld * Kd * max( dot(s, normalEC), 0.0 );
    
    // Final vertex position in NDC-Space
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(vertexPosition, 1.0);
}