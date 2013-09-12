#version 400

in vec3 position;
in vec3 normal;

layout (location = 0) out vec4 fragColor;

struct LightProperties {
    vec4 position;  // Light position in eye coordinate space.
    vec3 Ia;        // Ambient light intensity for each RGB component.
    vec3 Id;        // Diffuse light intensity for each RGB component.
    vec3 Is;        // Specular light intensity for each RGB component.
};
uniform LightProperties lightSource;

struct MaterialProperties {
    vec3 Ie;        // Emission light intensity from material for each RGB component.
    vec3 Ka;        // Coefficients of ambient reflectivity for each RGB component.
    vec3 Kd;        // Coefficients of diffuse reflectivity for each RGB component.
    vec3 Ks;        // Coefficients of specular reflectivity for each RGB component.
    float shininessFactor;   // Specular shininess factor.
};
uniform MaterialProperties material;

vec3 phongLightIntensity(vec4 position, vec3 normal) {
    vec3 s = normalize(vec3(lightSource.position - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect(-s, normal);

    vec3 ambient = lightSource.Ia * material.Ka;

    float sDotN = max(dot(s, normal), 0.0);
    vec3 diffuse = lightSource.Id * material.Kd * sDotN;
    
    vec3 specular = vec3(0.0);
    if (sDotN > 0.0) {
        specular = lightSource.Is * material.Ks * pow(max(dot(r, v), 0.0), material.shininessFactor); 
    }    
   
    return material.Ie + ambient + diffuse + specular;
}

void main() {
    fragColor = vec4( phongLightIntensity(vec4(position, 1.0), normal), 1.0 );
}
