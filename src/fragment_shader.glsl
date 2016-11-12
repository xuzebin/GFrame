uniform vec3 uColor;
uniform sampler2D uDiffuseTexture;
uniform sampler2D uSpecularTexture;
uniform float uMinColor;

varying vec3 vNormal;
varying vec2 vTexCoord;
varying vec3 vPosition;

struct Light {
    vec3 lightPosition;
    vec3 lightColor;
    vec3 specularLightColor;
};
uniform Light uLight[2];

float attenuate(float distance, float a, float b) {
    return 1.0 / (1.0 + a * distance + b * distance * distance);
}

void main() {
    vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
    vec3 specularColor = vec3(0.0, 0.0, 0.0);
    vec3 v = normalize(-vPosition);
    
    for (int i = 0; i < 2; ++i) {
        vec3 lightDirection = -normalize(vPosition - uLight[i].lightPosition);
        float diffuse = max(0.0, dot(vNormal, lightDirection));
        float attenuation = attenuate(distance(vPosition, uLight[i].lightPosition) / 50.0, 0.5, 0.5);
        diffuseColor += (uLight[i].lightColor * diffuse) * attenuation;
        
        vec3 h = normalize(v + lightDirection);
        float specular = pow(max(0.0, dot(h, vNormal)), 64.0);
        specularColor += (uLight[i].specularLightColor * specular);
        
    }
    vec3 intensity;
    if (uColor != vec3(-1, -1, -1)) {
        intensity = uColor * diffuseColor + specularColor;
    } else {
        intensity = texture2D(uDiffuseTexture, vTexCoord).xyz * diffuseColor + specularColor * texture2D(uSpecularTexture, vTexCoord).x;
    }
    gl_FragColor = vec4(intensity.xyz, 1.0);
    
}

