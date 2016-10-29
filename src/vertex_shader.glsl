attribute vec4 aPosition;
attribute vec4 aNormal;
attribute vec2 aTexCoord;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;

varying vec4 vNormal;
varying vec2 vTexCoord;

void main() {
    vNormal = normalize(uNormalMatrix * aNormal);
    vTexCoord = aTexCoord;
    //gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;
    gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;
}