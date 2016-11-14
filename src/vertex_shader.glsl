attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aNormal;
attribute vec4 aBinormal;
attribute vec4 aTangent;

varying mat3 vTBNMatrix;//for normal map

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
    
    vTBNMatrix = mat3(normalize((uNormalMatrix * aTangent).xyz), normalize((uNormalMatrix * aBinormal).xyz), normalize((uNormalMatrix * aNormal).xyz));
    
    gl_Position = uProjectionMatrix * viewspacePos;
}