
#include "SquidEngine.h"
#include "SampleShapes.h"
#include "Primitives.h"
#include "Shape.h"

using namespace std;

Square::Square(float x, float y, float z) : Shape(x, y, z) {}

//Build the Square
void Square::build() {

	vector<Vertex> vertices = {
			Vertex(-0.5f,  0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),   // top left 
			Vertex(0.5f,  0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),  // top right
			Vertex(0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),  // bottom right
			Vertex(-0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f)  // bottom left
	};

	vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3
	};

	createBuffer(vertices, indices);
}


Cube::Cube(float x, float y, float z) : Shape(x, y, z) {}

void Cube::build() {

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

	vector<Vertex> vertices = {
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

	vector<unsigned int> indices = {
		0,1,2,0,3,2,
		4,5,6,4,7,6,
		8,9,10,8,11,10,
		12,13,14,12,15,14,
		16,17,18,16,19,18,
		20,21,22,20,23,22
	};


	createBuffer(vertices, indices);
}


