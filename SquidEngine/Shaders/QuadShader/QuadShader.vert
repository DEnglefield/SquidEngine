#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;


out vec2 texUV;

void main() {
	texUV = tex;
    gl_Position = vec4(pos.xy, 0.0f, 1.0f);
}