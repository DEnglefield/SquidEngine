
#include "Cube.h"

//Smooth normals
glm::vec3 frontTopLeftNorm(-0.333f, 0.333f, 0.333f);
glm::vec3 frontTopRightNorm(0.333f, 0.333f, 0.333f);
glm::vec3 frontBottomRightNorm(0.333f, -0.333f, 0.333f);
glm::vec3 frontBottomLeftNorm(-0.333f, -0.333f, 0.333f);

glm::vec3 backTopLeftNorm(-0.333f, 0.333f, -0.333f);
glm::vec3 backTopRightNorm(0.333f, 0.333f, -0.333f);
glm::vec3 backBottomRightNorm(0.333f, -0.333f, -0.333f);
glm::vec3 backBottomLeftNorm(-0.333f, -0.333f, -0.333f);

//Texture sample coords
glm::vec2 texTopLeft(1.0f, 0.0f);
glm::vec2 texTopRight(1.0f, 1.0f);
glm::vec2 texBottomRight(0.0f, 1.0f);
glm::vec2 texBottomLeft(0.0f, 0.0f);

std::vector<Vertex> vertices = {
	//Front
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), frontTopLeftNorm, texTopLeft), //TopLeft
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), frontTopRightNorm, texTopRight), //TopRight
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), frontBottomRightNorm, texBottomRight), //BottomRight
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), frontBottomLeftNorm, texBottomLeft), //BottomLeft

	//Back
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), backTopLeftNorm, texTopRight), //TopLeft
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), backTopRightNorm, texTopLeft), //TopRight
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), backBottomRightNorm, texBottomLeft), //BottomRight
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), backBottomLeftNorm, texBottomRight), //BottomLeft

	//Top
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), backTopLeftNorm, texTopLeft), //TopLeft
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), backTopRightNorm, texTopRight), //TopRight
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), frontTopRightNorm, texBottomRight), //BottomRight
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), frontTopLeftNorm, texBottomLeft), //BottomLeft

	//Bottom
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), frontBottomLeftNorm, texTopLeft), //TopLeft
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), frontBottomRightNorm, texTopRight), //TopRight
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), backBottomRightNorm, texBottomRight), //BottomRight
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), backBottomLeftNorm, texBottomLeft), //BottomLeft

	//Left
	Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), backTopLeftNorm, texTopLeft), //TopLeft
	Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), frontTopLeftNorm, texTopRight), //TopRight
	Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), frontBottomLeftNorm, texBottomRight), //BottomRight
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), backBottomLeftNorm, texBottomLeft), //BottomLeft

	//Right
	Vertex(glm::vec3(0.5f, 0.5f, 0.5f), frontTopRightNorm, texTopLeft), //TopLeft
	Vertex(glm::vec3(0.5f, 0.5f, -0.5f), backTopRightNorm, texTopRight), //TopRight
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), backBottomRightNorm, texBottomRight), //BottomRight
	Vertex(glm::vec3(0.5f, -0.5f, 0.5f), frontBottomRightNorm, texBottomLeft) //BottomLeft
};

std::vector<unsigned int> indices = {
	0,2,1,0,3,2, //Front
	4,5,6,4,6,7, //Back
	8,10,9,8,11,10, //Top
	12,14,13,12,15,14, //Bottom
	16,18,17,16,19,18, //Left
	20,22,21,20,23,22 //Riht
};

Cube::Cube(float x, float y, float z) : Shape(x, y, z , vertices, indices) {}




