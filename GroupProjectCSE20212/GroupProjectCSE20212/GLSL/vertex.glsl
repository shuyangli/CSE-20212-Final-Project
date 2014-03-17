#version 120

attribute vec3 inputCoords;

void main() {
	gl_Position = vec4(inputCoords, 1.0f);
    
}