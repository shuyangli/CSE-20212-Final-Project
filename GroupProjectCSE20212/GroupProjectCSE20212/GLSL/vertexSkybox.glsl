#version 120

// input attributes for each vertex
attribute vec3 inputVertexPosModelSpace;
attribute vec3 inputVertexUV;
attribute vec3 inputVertexNormalModelSpace;

// output data for each fragment
varying vec2 varyUV;

// uniforms
uniform mat4 mvpMatrix;
uniform sampler2D skyboxTextureSampler;

void main() {
    varyUV = inputVertexUV;
	gl_Position = mvpMatrix * vec4(inputCoords, 1.0f);
}