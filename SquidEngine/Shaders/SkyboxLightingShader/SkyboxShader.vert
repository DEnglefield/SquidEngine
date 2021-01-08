#version 330 core

//Shader used to draw a skybox (Removed rotation from view matrix)

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

out vec3 tex3D;

void main() {
  
	tex3D = pos;
    //Convert position with no z to NDC to disregard depth value
    gl_Position = (projMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0)).xyww;
}