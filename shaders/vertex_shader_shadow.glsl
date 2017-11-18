attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aNormal;

uniform mat4 uModelViewMatrix;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;

uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uShadowMatrix;

varying vec3 vNormal;
varying vec2 vTexCoord;
varying vec3 vPosition;

void main() {
    vNormal = normalize((uNormalMatrix * aNormal).xyz);
    vTexCoord = aTexCoord;
//    vPosition = (uModelViewMatrix * aPosition).xyz;
    vPosition = (uViewMatrix * uShadowMatrix * uModelMatrix * aPosition).xyz;
    gl_Position = uProjectionMatrix * vec4(vPosition, 1.0);
}