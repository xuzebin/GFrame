uniform vec3 uColor;

varying vec3 vNormal;
varying vec2 vTexCoord;
varying vec3 vWorldPos;

uniform vec3 uCamPos;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform int gammaCorrect;
uniform int hdr;

struct Light {
    vec3 lightPosition;
    vec3 lightColor;
    vec3 specularLightColor;
    
};
uniform Light uLight[2];

const float PI = 3.14159265359;

float attenuate(float distance, float a, float b) {
    return 1.0 / (1.0 + a * distance + b * distance * distance);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nominator   = a2;
    float denominator = (NdotH2 * (a2 - 1.0) + 1.0);
    denominator       = PI * denominator * denominator;

    return nominator / denominator;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    
    float nominator   = NdotV;
    float denominator = NdotV * (1.0 - k) + k;
    
    return nominator / denominator;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    float ggx1 = geometrySchlickGGX(NdotL, roughness);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


void main() {
    
    vec3 albedo = uColor;
//    float metallic = 0.2;
//    float roughness = 0.5;
//    float ao = 1.0;

    vec3 N = vNormal;
    vec3 V = normalize(uCamPos - vWorldPos);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    
    //reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 2; ++i) {
        //calculate per-light radiance
        vec3 L = normalize(uLight[i].lightPosition - vWorldPos);
        vec3 H = normalize(V + L);
        float distance    = length(uLight[i].lightPosition - vWorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = uLight[i].lightColor * attenuation;

        //cook-torrance BRDF
        float NDF = distributionGGX(N, H, roughness);
        float G   = geometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD     *= (1.0 - metallic);
    
        float NdotL = max(dot(N, L), 0.0);
        vec3 nominator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL;
        vec3 specular = nominator / max(denominator, 0.001);

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    // HDR
    if (hdr == 1) {
        color = color / (color + vec3(1.0));
    }
    // Gamma correction
    if (gammaCorrect == 1) {
        color = pow(color, vec3(1.0/2.2));
    }
        
    gl_FragColor = vec4(color, 1.0);  
}

// void main() {
    
//     vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
//     vec3 specularColor = vec3(0.0, 0.0, 0.0);
    

//     for (int i = 0; i < 2; ++i) {
//         vec3 lightDirection = -normalize(vWorldPos - uLight[i].lightPosition);
//         float diffuse = max(0.0, dot(vNormal, lightDirection));
//         float attenuation = attenuate(distance(vWorldPos, uLight[i].lightPosition) / 100.0, 2.7, 5.0);//30.0, 0.5, 0.5);
//         diffuseColor += (uLight[i].lightColor * diffuse) * attenuation;
        
//         vec3 v = normalize(-vWorldPos);
//         vec3 h = normalize(v + lightDirection);
//         float specular = pow(max(0.0, dot(h, vNormal)), 64.0);
//         specularColor += (uLight[i].specularLightColor * specular) * attenuation;
//     }
//     vec3 intensity = uColor * diffuseColor + specularColor;
    
//     gl_FragColor = vec4(intensity, 1.0);
// }

