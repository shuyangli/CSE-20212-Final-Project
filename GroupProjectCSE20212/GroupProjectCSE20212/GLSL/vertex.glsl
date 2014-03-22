#version 120

attribute vec3 inputCoords;

uniform mat4 mvpMatrix;

void main() {
    
	gl_Position = mvpMatrix * vec4(inputCoords, 1.0f);
    
}