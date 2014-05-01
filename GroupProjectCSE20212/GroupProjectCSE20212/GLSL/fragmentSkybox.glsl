#version 120

// varyings
varying vec2 UV;

uniform sampler2D textureSampler;

void main() {
//    vec2 UV_Flipped = UV;
//    UV_Flipped.y = 1 - UV_Flipped.y;
	gl_FragColor = texture2D(textureSampler, UV);
}