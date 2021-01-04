#pragma once

#include <vector>
#include "Primitives.h"

//A quad to be presented on the screen
class ScreenQuad {
private:
	//ID of texture to display
	unsigned int textureID;
	//Vertex buffers for quad
	unsigned int VBO, EBO, VAO;

	std::vector<Vertex> vertices;
	unsigned int indices[6] = { 0,2,1,2,0,3 };

	//Create buffers for the quad
	void createBuffer();
public:
	ScreenQuad();
	//Create a screen quad using a position and size between 0 and 1
	ScreenQuad(float xPos, float yPos, float width, float height);
	//Set a texture to draw on the quad
	void setTexture(unsigned int texture);
	//Draw the quad with the QuadShader
	void draw();
	//Free used buffers
	void destroy();
};