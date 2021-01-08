#version 330 core

//Minimal shader to draw objects on the scene

//Input vertex attributes
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

//Input matrices
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 texUV;

void main()
{
	mat4 viewTransform = projMatrix * viewMatrix;
	fragNormal = mat3(transpose(inverse(modelMatrix))) * normal;
	texUV = tex;
	fragPos = vec3(modelMatrix * vec4(pos, 1.0));
    gl_Position = viewTransform * vec4(fragPos, 1.0);
}