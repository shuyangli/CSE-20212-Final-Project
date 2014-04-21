#version 120

// input attributes for each vertex
attribute vec3 inputVertexPosModelSpace;
attribute vec3 inputVertexNormalModelSpace;

// output data for each fragment
varying vec3 varyPosWorldSpace;
varying vec3 varyNormalCameraSpace;
varying vec4 varyLightWithoutColor;

// uniforms
uniform mat4 mvpMatrix;
uniform mat3 normalModelViewMatrix;

// for lighting
uniform vec3 directionToLight;
uniform vec4 directionalLightIntensity;
uniform vec4 ambientLightIntensity;

void main() {
    
	gl_Position = mvpMatrix * vec4(inputVertexPosModelSpace, 1.0f);
    
    vec3 normalCamSpace = normalize(normalModelViewMatrix * inputVertexNormalModelSpace);
    float incidence = dot(normalCamSpace, directionToLight);
    incidence = clamp(incidence, 0, 1);
    
    varyLightWithoutColor = directionalLightIntensity * incidence + ambientLightIntensity;
    
    // fundamental idea:
    // interpColor = (diffuseColor * lightIntensity * cosAngIncidence)
    // + (diffuseColor * ambientIntensity);
    
}