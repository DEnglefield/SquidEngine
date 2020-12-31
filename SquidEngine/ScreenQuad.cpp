

#include "ScreenQuad.h"
#include <glad\glad.h>


ScreenQuad::ScreenQuad() {
	vertices.push_back(Vertex(glm::vec3(-1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 1)));
	vertices.push_back(Vertex(glm::vec3(1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(1, 1)));
	vertices.push_back(Vertex(glm::vec3(1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(1, 0)));
	vertices.push_back(Vertex(glm::vec3(-1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)));
	createBuffer();
}

//Create a screen quad using a position and size between 0 and 1
ScreenQuad::ScreenQuad(float xPos, float yPos, float width, float height) {
	xPos -= 1;
	yPos -= 1;
	width *= 2;
	height *= 2;

	vertices.push_back(Vertex(glm::vec3(xPos, yPos + height,0), glm::vec3(0, 0, 0), glm::vec2(0, 1)));
	vertices.push_back(Vertex(glm::vec3(xPos + width, yPos + height, 0), glm::vec3(0, 0, 0), glm::vec2(1, 1)));
	vertices.push_back(Vertex(glm::vec3(xPos + width, yPos, 0), glm::vec3(0, 0, 0), glm::vec2(1, 0)));
	vertices.push_back(Vertex(glm::vec3(xPos, yPos, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)));
	createBuffer();
}


//Draw the quad with the QuadShader
void ScreenQuad::draw() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0); //Need default texture ID
}


//Set a texture to draw on the quad
void ScreenQuad::setTexture(unsigned texture) { textureID = texture; }


//Create buffers for the quad
void ScreenQuad::createBuffer() {
	//Generate unique IDs for buffer objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Populate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

	//Populate Element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);

	//Populate attribute buffer using vertex attributes
	Vertex::getAttributeDescription();

	glBindVertexArray(0);
}