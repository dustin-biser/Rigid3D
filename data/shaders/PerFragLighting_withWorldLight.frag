// PerFragLighting_withWorldLight.frag
#version 420
#extension GL_ARB_explicit_uniform_location : enable

in vec3 position;
in vec3 normal;

out layout (location = 0) vec4 fragColor;

struct Material {
    vec3 emission;  // Emission light intensity from material for each RGB component.
    vec3 Ka;        // Coefficients of ambient reflectivity for each RGB component.
    vec3 Kd;        // Coefficients of diffuse reflectivity for each RGB component.
    float Ks;       // Coefficient of specular reflectivity, uniform across each RGB component.
    float shininess;  // Specular shininess factor.
};
layout (location = 3) uniform Material material;

const uint MAX_NUM_LIGHTS = 10;
const uint LIGHT_TYPE_DIRECTIONAL = 0;
const uint LIGHT_TYPE_POINT = 1;
struct Light {
    uint type;      // The type of Light (0 = Directional, 1 = Point).
    vec3 position;  // Light position in eye coordinate space.
    vec3 direction; // Direction the light is pointing.
    vec3 color;     // Light intensity for each RGB component.
    bool isEnabled; // True if Light contributes to this fragment's color.
};
layout (location = 8) uniform Light light[MAX_NUM_LIGHTS];

uniform vec3 ambientIntensity; // Environmental ambient light intensity for each RGB component.


vec3 computePhong(vec3 fragPosition, vec3 fragNormal, uint index) {
    vec3 l = normalize(light[index].position - fragPosition); // Direction from fragment to light source.
    vec3 v = normalize(-fragPosition); // Direction from fragment to viewer (origin - fragPosition).
    vec3 h = normalize(v + l); // Halfway vector.

    vec3 ambient = ambientIntensity * material.Ka;

    float n_dot_l = max(dot(fragNormal, l), 0.0);
    vec3 diffuse = material.Kd * n_dot_l;
    
    vec3 specular = vec3(0.0);
    if (n_dot_l > 0.0) {
        float n_dot_h = max(dot(fragNormal, h), 0.0);
        specular = vec3(material.Ks * pow(n_dot_h, material.shininess)); 
    }    
   
    return material.emission + ambient + light[index].color * (diffuse + specular);
}

void main() {
    fragColor = vec4(0);
   
    // Sum the contributions from all lights
    for(uint i = 0; i < MAX_NUM_LIGHTS; i++) { 
        if (!light[i].isEnabled) continue;

	    fragColor += vec4(computePhong(position, normal, i), 1.0);
    }
}
