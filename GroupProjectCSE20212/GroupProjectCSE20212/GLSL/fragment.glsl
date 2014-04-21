#version 120

varying vec4 varyLightWithoutColor;

void main() {
    
    vec4 color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
	gl_FragColor = varyLightWithoutColor * color;
}