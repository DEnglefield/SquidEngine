
#include "SampleShapes.h"
#include "Primitives.h"
#include "Shape.h"

using namespace std;

Square::Square(float x, float y, float z) : Shape(x, y, z) {}

//Build the Square
void Square::build() {

	vector<Vertex> vertices = {
			Vertex(0.5f,  0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f),  // top right
			Vertex(0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f),  // bottom right
			Vertex(-0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f),  // bottom left
			Vertex(-0.5f,  0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f)   // top left 
	};

	vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3
	};

	createBuffer(vertices, indices);
}
