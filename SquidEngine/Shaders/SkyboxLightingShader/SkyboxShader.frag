#version 330 core
out vec4 FragColor;

//Shader to draw a skybox by sampling cubemap texture

in vec3 tex3D;

uniform samplerCube skyboxTexture;

void main() {    
    FragColor = texture(skyboxTexture, tex3D);
}

