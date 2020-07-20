#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

uniform mat4 modelMatrix;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec3 fragNormal;
out vec2 texUV;

void main()
{
	mat4 fullTransform = projMatrix * viewMatrix * worldMatrix * modelMatrix;
	fragNormal = normal;
	texUV = tex;
    gl_Position = fullTransform * vec4(pos, 1.0);
}