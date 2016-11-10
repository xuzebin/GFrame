attribute vec4 aPosition;
attribute vec4 aNormal;
attribute vec2 aTexCoord;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;

varying vec3 vNormal;
varying vec2 vTexCoord;
varying vec3 vPosition;

void main() {
    vNormal = normalize((uNormalMatrix * aNormal).xyz);
    vTexCoord = aTexCoord;
    vec4 viewspacePos = uModelViewMatrix * aPosition;
    vPosition = viewspacePos.xyz;
    gl_Position = uProjectionMatrix * viewspacePos;
}