uniform vec3 uColor;
uniform sampler2D uTexture;
uniform float uMinColor;

varying vec4 vNormal;
varying vec2 vTexCoord;

void main() {
    float diffuse = uMinColor + max(0.0, dot(vNormal, vec4(-0.5773, 0.5773, 0.5773, 0.0)));
//    vec3 intensity = uColor * diffuse;
    vec3 intensity;
    if (uColor != vec3(-1, -1, -1)) {
        intensity = uColor * diffuse;
    } else {
        intensity = texture2D(uTexture, vTexCoord).xyz * diffuse;
    }
//    vec3 intensity = texture2D(uTexture, vTexCoord).xyz * diffuse;
    gl_FragColor = vec4(intensity.xyz, 1.0);
}