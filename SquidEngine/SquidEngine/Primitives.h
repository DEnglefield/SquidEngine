#pragma once

#include "SquidEngine.h"

//Vertex used to represent points in 3D space including
//Position, normal and colour
struct Vertex {
	glm::vec3 pos, normal, colour;
	//Create vertex using a vector for each attribute
	Vertex(glm::vec3 position, glm::vec3 norm, glm::vec3 col);
	//Create a vertex using a list of raw values
	Vertex(float vX, float vY, float vZ, float nX, float nY, float nZ, float R, float G, float B);
	//Get a description of the location of each attribute in the vertex
	static void getAttributeDescription();
};