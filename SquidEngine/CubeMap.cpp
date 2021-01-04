
#include "stb_image.h"
#include <glad\glad.h>
#include "CubeMap.h"

#include <iostream>

CubeMap::CubeMap(std::vector<std::string>& cubeImages) {
	generateCubeMap(cubeImages);
}


void CubeMap::generateCubeMap(std::vector<std::string>& cubeImages) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    stbi_set_flip_vertically_on_load(false);
    int width, height, numChannels;
    unsigned char* data;
    for (unsigned int i = 0; i < 6; i++) {
        data = stbi_load(cubeImages[i].c_str(), &width, &height, &numChannels, 0);


        if (data) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
        }
        else {
            std::cout << "Failed to load texture: " << cubeImages[i].c_str() << std::endl;
            throw std::exception("Failed to load CubeMap texture");

        }
       
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}


void CubeMap::destroy() {
    glDeleteTextures(1, &textureID);
}


void SkyBox::destroy() {
    CubeMap::destroy();
    Cube::destroy();
}