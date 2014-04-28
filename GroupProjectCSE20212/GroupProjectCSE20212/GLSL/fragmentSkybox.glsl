#version 120

// varyings
varying vec2 UV;

uniform sampler2D textureSampler;

void main() {
	gl_FragColor = texture2D(textureSampler, UV);
}