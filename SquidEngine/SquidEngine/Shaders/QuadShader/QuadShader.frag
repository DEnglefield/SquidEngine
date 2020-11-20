#version 330 core
out vec4 FragColor;

uniform sampler2D quadOutputTexture;
in vec2 texUV;

void main() {
	//FragColor = FragColor = vec4(1.0 - texture(quadOutputTexture, texUV).xyz, 1.0);
	FragColor = texture(quadOutputTexture, texUV);
} 


