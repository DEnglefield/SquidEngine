#version 330 core
out vec4 FragColor;

in vec3 tex3D;

uniform samplerCube skyboxTexture;

void main() {    
    FragColor = texture(skyboxTexture, tex3D);
}

