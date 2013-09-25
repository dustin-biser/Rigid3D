#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

struct LightSource {
    vec3 position;      // Light position in world space.
    vec3 rgbIntensity;  // Light intensity for each RGB component.
};
uniform LightSource lightSource;

out vec3 position;
out vec3 normal;
out vec3 lightPos;
out vec3 lightRgb;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    // Transform vertex position and normal to eye coordinate space.
    normal = normalize(NormalMatrix * vertexNormal);
    position = vec3( ViewMatrix * ModelMatrix * vec4(vertexPosition, 1.0) );
    
    lightPos = vec3(ViewMatrix * vec4(lightSource.position, 1.0));
    lightRgb = lightSource.rgbIntensity;

    // Transform position to normalized device coordinate space.
    gl_Position = ProjectionMatrix * vec4(position, 1.0);
}