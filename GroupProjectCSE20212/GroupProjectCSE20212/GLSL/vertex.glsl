#version 120

attribute vec3 inputCoords;
attribute vec3 inputNormal;

varying vec4 lightWithoutColor;

uniform mat4 mvpMatrix;
uniform mat3 normalModelViewMatrix;

// for lighting
uniform vec3 directionToLight;
uniform vec4 lightIntensity;

void main() {
    
	gl_Position = mvpMatrix * vec4(inputCoords, 1.0f);
    
    vec3 normalCamSpace = normalize(normalModelViewMatrix * inputNormal);
    float incidence = dot(normalCamSpace, directionToLight);
    incidence = clamp(incidence, 0, 1);
    
    lightWithoutColor = lightIntensity * incidence;
    
    // fundamental idea:
    // interpColor = (diffuseColor * lightIntensity * cosAngIncidence)
    // + (diffuseColor * ambientIntensity);
    
}