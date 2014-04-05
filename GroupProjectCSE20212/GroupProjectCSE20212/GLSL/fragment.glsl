#version 120

varying vec4 lightWithoutColor;

void main() {
    
	gl_FragColor = lightWithoutColor * vec4(0.5f, 0.5f, 0.5f, 1.0f);
}