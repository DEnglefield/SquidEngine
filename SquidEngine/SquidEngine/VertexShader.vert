#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tex;

uniform mat4 modelMatrix;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec3 normalOut;

void main()
{
	mat4 fullTransform = projMatrix * viewMatrix * worldMatrix * modelMatrix;
	normalOut = normal;
    gl_Position = fullTransform * vec4(pos, 1.0);
}