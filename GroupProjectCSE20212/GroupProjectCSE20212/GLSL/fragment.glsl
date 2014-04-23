#version 120

// varyings
//varying vec4 varyLightWithoutColor;
varying vec3 normalCamSpace;

// uniforms for lighting
uniform vec4 materialColor;
uniform vec3 directionToLight;
uniform vec4 directionalLightIntensity;
uniform vec4 ambientLightIntensity;

void main() {
  
    // previously used
    float dist = 1.0f;
    
    float incidence = dot(normalCamSpace, directionToLight);
    incidence = clamp(incidence, 0, 1);
    vec4 lightWithoutColor = directionalLightIntensity * incidence + ambientLightIntensity;
    
    vec4 color = lightWithoutColor * materialColor / (dist * dist);
    
	gl_FragColor = color;
}