#pragma once

#include "SquidEngine.h"
#include "Camera.h"

//A camera capable of moving around the scene
class CameraFPS : public Camera {
protected:
	//Speed of camera movement after adjusting for current FPS
	float adjustedSpeed = moveSpeed;
	
	bool hasLastCursor = false;
	glm::vec2 lastCursorPos;

	//Camera rotation angles
	float pitch, yaw;

	//Sensitivity of mouse cursor movement
	float sensitivity = 0.09;
public:
	//Distance per second
	float moveSpeed = 2.5f; 

	//Create a camera using eye and look target vector
	CameraFPS(float eyeX, float eyeY, float eyeZ,
		float lookX, float lookY, float lookZ);
	CameraFPS(glm::vec3 eye, glm::vec3 look);

	//Move the camera forward
	void moveForward();
	//Move the camera backward
	void moveBackward();
	//Move the camera left
	void moveLeft();
	//Move the camera right
	void moveRight();
	//Move the camera up
	void moveUp();
	//Move the camera down
	void moveDown();

	//Update the adjusted speed of the camera movement using FPS
	void updateFPS(float FPS);

	//Set the camera mouse movement sensitivity
	void setSensitivity(float cameraSensitivity);

	//Rotate the camera to match the current cursor position
	void updateCursorPos(double xPos, double yPos, bool doRotation);
};
