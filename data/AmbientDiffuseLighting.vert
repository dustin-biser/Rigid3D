#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 fragColor;

uniform vec3 lightPositionEC; // Light position in eye coordinates.
uniform vec3 Ia;  // Ambient  light intensity for RGB color spectrum.
uniform vec3 Id;  // Diffuse light intensity for RGB color spectrum.

uniform vec3 Ka;  // Coefficient of ambient reflectivity for RGB specturm.
uniform vec3 Kd;  // Coefficient of diffuse reflectivity for RGB spectrum.

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

void main() {
    // Transformed normal given in eye coordinates.
    vec3 normalEC = normalize(NormalMatrix * vertexNormal);
    
    // Transformed vertex position given in eye coordinates.
    vec4 vertexPositionEC = ModelViewMatrix * vec4(vertexPosition, 1.0);
    
    // Light source vector
    vec3 s = normalize(lightPositionEC - vec3(vertexPositionEC));
    
    vec3 ambient = Ia * Ka;
    
    vec3 diffuse = Id * Kd * max( dot(s, normalEC), 0.0);
    
    // Diffuse shading equation
    fragColor = ambient + diffuse;
    
    // Final vertex position in NDC-Space
    gl_Position = ProjectionMatrix * vertexPositionEC;
}