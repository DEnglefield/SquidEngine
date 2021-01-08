#version 330 core
out vec4 FragColor;

uniform sampler2D renderTexture;
uniform float gamma;
in vec2 texUV;

void main() {
	FragColor = vec4(texture(renderTexture, texUV).xyz,1.0);
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
} 


