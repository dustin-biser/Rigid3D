// ADS_withShadow.frag
#version 400

in vec3 position;
in vec3 normal;
in vec4 shadowCoord;

layout (location = 0) out vec4 fragColor;

uniform vec3 ambientIntensity; // Environmental ambient light intensity for each RGB component.
uniform sampler2DShadow shadowMap;

struct LightSource {
    vec3 position;      // Light position in eye coordinate space
    vec3 rgbIntensity;  // Light intensity for each RGB component.
};
uniform LightSource light;

struct MaterialProperties {
    vec3 emission;  // Emission light intensity from material for each RGB component.
    vec3 Ka;        // Coefficients of ambient reflectivity for each RGB component.
    vec3 Kd;        // Coefficients of diffuse reflectivity for each RGB component.
    float Ks;       // Coefficient of specular reflectivity, uniform across each RGB component.
    float shininess;  // Specular shininess factor.
};
uniform MaterialProperties material;

vec3 phongModelNoAmbient()
{
    vec3 n = normal;
    if (!gl_FrontFacing)  n = -n;
    vec3 s = normalize(light.position - position);
    vec3 v = normalize(-position.xyz);
    vec3 h = normalize(v + s); // Halfway vector.
    float s_dot_n = max( dot(s,n), 0.0 );
    vec3 diffuse = light.rgbIntensity * material.Kd * s_dot_n;
    vec3 specular = vec3(0.0);
    if (s_dot_n > 0.0) {
        float h_dot_n = max(dot(h, n), 0.0);
        specular = light.rgbIntensity * vec3(material.Ks * pow(h_dot_n, material.shininess));
    }

    return material.emission + diffuse + specular;
}

subroutine void RenderPassType();
subroutine uniform RenderPassType renderPass;

subroutine (RenderPassType)
void shadeWithShadow() {
    vec3 ambient = ambientIntensity * material.Ka;
    vec3 diffAndSpec = phongModelNoAmbient();
    
    float shadow = textureProj(shadowMap, shadowCoord);
    
    // Use ambient light only, if the fragment is in shadow.
    if (shadow == 0) {
        shadow = 0.35f;
    }
    
    fragColor = vec4(diffAndSpec * shadow + ambient, 1.0);
    
    // Gamma correction
    fragColor = pow(fragColor, vec4(1.0 / 1.5));
}

subroutine (RenderPassType)
void recordDepthValues() {
    // Do nothing, as the depth will be automatically written.
}

void main() {
    // This will call either recordDepth() or shadeWithShadow().
    renderPass();
}
