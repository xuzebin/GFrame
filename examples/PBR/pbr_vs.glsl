attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aNormal;

uniform mat4 uModelMatrix;//new uniform
uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;

varying vec3 vNormal;
varying vec2 vTexCoord;
varying vec3 vWorldPos;//new varying

void main() {
    vNormal = normalize((uNormalMatrix * aNormal).xyz);
    vTexCoord = aTexCoord;
    vWorldPos = (uModelMatrix * aPosition).xyz;
//    vWorldPos = (uModelViewMatrix * aPosition).xyz;
//    vCamPos = (uModelViewMatrix * aPosition).xyz;

//    gl_Position = uProjectionMatrix * vec4(vCamPos, 1.0); 
    gl_Position = uProjectionMatrix * uModelViewMatrix * aPosition;
}