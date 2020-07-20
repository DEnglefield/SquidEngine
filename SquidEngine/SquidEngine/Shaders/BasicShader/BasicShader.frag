#version 330 core
out vec4 FragColor;

uniform sampler2D textureIn;
in vec3 fragNormal;
in vec2 texUV;

uniform vec4 colour;

void main() {
	FragColor = colour * texture(textureIn, texUV);
} 