// ADS_shadow_spotlight.frag
#version 400

in vec3 position;
in vec3 normal;
in vec4 shadowCoord;

layout (location = 0) out vec4 fragColor;

uniform vec3 ambientIntensity; // Environmental ambient light intensity for each RGB component.
uniform sampler2DShadow shadowMap;

struct SpotLight {
    vec3 position;  // Position in eye coordinate space.
    vec3 center;  // Where spot light is center on in eye coordinate space.
    vec3 rgbIntensity;  // Light intensity for each RGB component.
    float exponent;  // Angular attenuation exponent.
    float conicAngle;  // Half angle of light cone (between 0 and 90 degrees).
};
uniform SpotLight spotLight;

struct MaterialProperties {
    vec3 emission;  // Emission light intensity from material for each RGB component.
    vec3 Ka;        // Coefficients of ambient reflectivity for each RGB component.
    vec3 Kd;        // Coefficients of diffuse reflectivity for each RGB component.
    float Ks;       // Coefficient of specular reflectivity, uniform across each RGB component.
    float shininess;  // Specular shininess factor.
};
uniform MaterialProperties material;

// in vec s - Normalized direction from fragment to SpotLight.
void phongModelDiffAndSpec(vec3 s, out vec3 diffuse, out vec3 specular) {
    vec3 n = normal;
    if (!gl_FrontFacing)  n = -n;
    vec3 v = normalize(-position.xyz);
    vec3 h = normalize(v + s); // Halfway vector.
    float s_dot_n = max( dot(s,n), 0.0 );
    diffuse = spotLight.rgbIntensity * material.Kd * s_dot_n;
    specular = vec3(0.0);
    if (s_dot_n > 0.0) {
        float h_dot_n = max(dot(h, n), 0.0);
        specular = spotLight.rgbIntensity * vec3(material.Ks * pow(h_dot_n, material.shininess));
    }
}

subroutine void RenderPassType();
subroutine uniform RenderPassType renderPass;

subroutine (RenderPassType)
void shadeWithShadow() {
    vec3 ambient = ambientIntensity * material.Ka;

    vec3 s = normalize(spotLight.position - position);
    vec3 direction = normalize(spotLight.center - spotLight.position);
    float angle = acos( dot(-s, direction) );
    float conicAngle = radians( clamp(spotLight.conicAngle, 0.0, 90.0) );

    vec3 diffuse;
    vec3 specular;
    if (angle < conicAngle) {
       float spotFactor = pow( dot(-s, direction), spotLight.exponent );
	   phongModelDiffAndSpec(s, diffuse, specular);
	   diffuse *= spotFactor;
	   specular *= spotFactor;
    }
    
    float sum = 0;
    // If fragment is in shadow, textureProjOffset returns 0, otherwise it returns 1.
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(-1, -1));
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(-1, 1));
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(1, -1));
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(1, 1));
    
    sum = sum * 0.25;

    float diminishFactor = 0.35;
    if (sum == 0) {
        diffuse *= (1-sum) * diminishFactor + sum;
        specular = vec3(0.0);
    }
    else if (sum < 1) {
        diffuse *= (1-sum) * diminishFactor + sum;
        specular *= (1-sum) * diminishFactor + sum;
    }
    
    fragColor = vec4(diffuse + specular + ambient + material.emission, 1.0);
    
    // Gamma correction
    fragColor = pow(fragColor, vec4(0.66));
}

subroutine (RenderPassType)
void recordDepthValues() {
    // Do nothing, as the depth will be automatically written.
}

void main() {
    // This will call either recordDepth() or shadeWithShadow().
    renderPass();
}
