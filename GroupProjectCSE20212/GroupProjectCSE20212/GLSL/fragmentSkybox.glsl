#version 120

uniform sampler2D skyboxTextureSampler;

varying vec2 varyUV;

void main() {
	gl_FragColor = texture(skyboxTextureSampler, varyUV);
}