#version 120

// input attributes for each vertex
attribute vec3 inputVertexPosModelSpace;
attribute vec2 inputVertexUVModelSpace;

// output data for each fragment
varying vec2 UV;

// uniforms
uniform mat4 mvpMatrix;

void main() {
    
	gl_Position = mvpMatrix * vec4(inputVertexPosModelSpace, 1.0f);
    UV = inputVertexUVModelSpace;
}