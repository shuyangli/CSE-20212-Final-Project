#version 120

// varyings
varying vec2 UV;

uniform sampler2D textureSampler;

void main() {
    
    vec4 color = lightWithoutColor * materialColor / (dist * dist);
	gl_FragColor = texture(texturSampler, UV).rgb;
}