
#include "Object3D.h"

//Add a rotation to the object
void Object3D::rotate(float angle, float x, float y, float z) {
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(x, y, z));
}


//Set the position of the object
void Object3D::setPosition(float x, float y, float z) {
	modelMatrix[3][0] = x;
	modelMatrix[3][1] = y;
	modelMatrix[3][2] = z;
	modelMatrix[3][3] = 1.0f;
}

//Get the position of the object
glm::vec3 Object3D::getPosition() {
	return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
}

//Scale the object on the chosen axis
void Object3D::setScale(float x, float y, float z) {
	modelMatrix = glm::scale(modelMatrix, glm::vec3(x, y, z));
}

//Get the current scale of the object
glm::vec3 Object3D::getScale() {
	return glm::vec3(modelMatrix[0][0], modelMatrix[1][1], modelMatrix[2][2]);
}

//Move the object along the given axis
void Object3D::translate(float x, float y, float z) {
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
}