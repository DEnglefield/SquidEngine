#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Abstract class implementing transformation controls for objects in the scene
class Object3D {
protected:
	//Model matrix to hold object transformations
	glm::mat4 modelMatrix;

public:

	//Require an initial position when creating an object
	Object3D(float posX, float posY, float posZ) { modelMatrix = glm::mat4(1.0f);  setPosition(posX, posY, posZ); }

	//Set the position of the object
	void setPosition(float x, float y, float z);
	//Get the position of the object
	glm::vec3 getPosition();

	//Scale the object on the chosen axis
	void setScale(float x, float y, float z);
	//Get the current scale of the object
	glm::vec3 getScale();
	//Add a rotation to the object
	void rotate(float angle, float x, float y, float z);
	//Move the object along the given axis
	void translate(float x, float y, float z);

	//Return the shape's model matrix
	inline glm::mat4 getModelMatrix() { return modelMatrix; }
};