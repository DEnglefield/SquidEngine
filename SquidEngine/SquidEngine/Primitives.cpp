
#include "Primitives.h"

//Create vertex using a vector for each attribute
Vertex::Vertex(glm::vec3 position, glm::vec3 norm, glm::vec3 col) {
	pos = position; normal = norm; colour = col;
}

//Create a vertex using a list of raw values
Vertex::Vertex(float vX, float vY, float vZ, float nX, float nY, float nZ, float R, float G, float B) {
	pos = glm::vec3(vX, vY, vZ);
	normal = glm::vec3(nX, nY, nZ);
	colour = glm::vec3(R, G, B);
}

//Get a description of the location of each attribute in the vertex
void Vertex::getAttributeDescription() {
	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	//Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Colour attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

