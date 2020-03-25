#pragma once

#include "SquidEngine.h"
#include <iostream>
//Vertex used to represent points in 3D space including
//Position, normal and texture coordinates
struct Vertex {
	glm::vec3 pos, normal;
	glm::vec2 texture;
	//Create vertex using a vector for each attribute
	Vertex(glm::vec3 position, glm::vec3 norm, glm::vec2 tex);
	//Create a vertex using a list of raw values
	Vertex(float vX, float vY, float vZ, float nX, float nY, float nZ, float tU, float tV);
	//Get a description of the location of each attribute in the vertex
	static void getAttributeDescription();

	//Print the vertex contents in string stream
	friend std::ostream& operator<<(std::ostream& os, const Vertex &vert);
};