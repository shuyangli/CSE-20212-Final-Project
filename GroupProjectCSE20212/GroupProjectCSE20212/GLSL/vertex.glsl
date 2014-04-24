#version 120

// input attributes for each vertex
attribute vec3 inputVertexPosModelSpace;
attribute vec3 inputVertexNormalModelSpace;

// output data for each fragment
//varying vec3 varyPosWorldSpace;
//varying vec3 varyNormalCameraSpace;
varying vec3 normalCamSpace;

// uniforms
uniform mat4 mvpMatrix;
uniform mat3 normalModelViewMatrix;

void main() {
    
	gl_Position = mvpMatrix * vec4(inputVertexPosModelSpace, 1.0f);
    normalCamSpace = normalize(normalModelViewMatrix * inputVertexNormalModelSpace);
}