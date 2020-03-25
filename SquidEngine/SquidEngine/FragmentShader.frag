#version 330 core
out vec4 FragColor;

in vec3 normal;

uniform vec4 col;

void main()
{
	FragColor = col;
} 