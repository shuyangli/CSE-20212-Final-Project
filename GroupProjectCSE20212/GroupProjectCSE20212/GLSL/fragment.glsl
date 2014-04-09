#version 120

varying vec4 lightWithoutColor;

void main() {
    
	gl_FragColor = lightWithoutColor * vec4(0.6f, 0.6f, 0.6f, 1.0f);
}