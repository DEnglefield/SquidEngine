#version 330 core

//Shader for creating a shadow map for a single light source

//Vertex attributes
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

//UBO containing matrices to reach NDC space
layout (std140) uniform ImageMatrices {
    mat4 viewMatrix;
    mat4 projMatrix; 
};

uniform mat4 modelMatrix;

void main() {
	//Transform vertices to NDC and pass required attributes
	mat4 viewTransform = projMatrix * viewMatrix;
    gl_Position = viewTransform * modelMatrix * vec4(pos, 1.0);
}