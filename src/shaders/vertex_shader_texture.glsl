attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aNormal;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;

varying vec3 vNormal;
varying vec2 vTexCoord;
varying vec3 vPosition;

void main() {
    vNormal = normalize((uNormalMatrix * aNormal).xyz);
    
    vTexCoord = aTexCoord;
    vec4 vPos4 = uModelViewMatrix * aPosition;
    vPosition = vPos4.xyz;
    
    gl_Position = uProjectionMatrix * vPos4;
}