#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;


out vec3 tex3D;

void main() {
	tex3D = pos;
    gl_Position = (projMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0)).xyww;
}