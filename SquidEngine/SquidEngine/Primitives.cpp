
#include "Primitives.h"

//Create vertex using a vector for each attribute
Vertex::Vertex(glm::vec3 position, glm::vec3 norm, glm::vec2 tex) {
	pos = position; normal = norm; texture = tex;
}

//Create a vertex using a list of raw values
Vertex::Vertex(float vX, float vY, float vZ, float nX, float nY, float nZ, float tU, float tV) {
	pos = glm::vec3(vX, vY, vZ);
	normal = glm::vec3(nX, nY, nZ);
	texture = glm::vec2(tU, tV);
}

//Print the vertex contents in string stream
std::ostream& operator<<(std::ostream& os, const Vertex& vert) {
	os << "((" << vert.pos.x << ", " << vert.pos.y << ", " << vert.pos.z << "), ";
	os << "(" << vert.normal.x << ", " << vert.normal.y << ", " << vert.normal.z << "), ";
	os << "(" << vert.texture.x << ", " << vert.texture.y << "))";
	return os;
}


//Get a description of the location of each attribute in the vertex
void Vertex::getAttributeDescription() {
	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	//Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}


//Calculate vertex triangle normal
glm::vec3 Vertex::calculateNormal(Vertex& other, Vertex& other2) {
	glm::vec3 side1 = other.pos - pos;
	glm::vec3 side2 = other2.pos - pos;

	return normalize(cross(side1, side2));

}




