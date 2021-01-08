#version 330 core

//Shader used to implement blinn-phong lighting and sample from skybox reflections

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

out vec3 fragPos;
out vec3 fragNormal;
out vec2 texUV;

void main() {
	//Transform vertices to NDC and pass required attributes
	mat4 viewTransform = projMatrix * viewMatrix;
	fragNormal = mat3(transpose(inverse(modelMatrix))) * normal;
	texUV = tex;
	fragPos = vec3(modelMatrix * vec4(pos, 1.0));
    gl_Position = viewTransform * vec4(fragPos, 1.0);
}