
#include "CameraFPS.h"


//Create a camera using eye and look target vector
CameraFPS::CameraFPS(float eyeX, float eyeY, float eyeZ,float lookX, float lookY, float lookZ)
	: Camera(eyeX, eyeY, eyeZ, lookX, lookY, lookZ) {}

//Create a camera using eye position and look/target vector as vectors
CameraFPS::CameraFPS(glm::vec3 eye, glm::vec3 look) : Camera(eye, look) {}

//Move the camera forward
void CameraFPS::moveForward() { eyePos += adjustedSpeed * lookVec; }

//Move the camera backward
void CameraFPS::moveBackward() { eyePos -= adjustedSpeed * lookVec; }

//Move the camera left
void CameraFPS::moveLeft() {
	eyePos -= glm::normalize(glm::cross(lookVec, upVec)) * adjustedSpeed;
}

//Move the camera right
void CameraFPS::moveRight() {
	eyePos += glm::normalize(glm::cross(lookVec, upVec)) * adjustedSpeed;
}

//Move the camera up
void CameraFPS::moveUp() {
	eyePos += glm::normalize(glm::cross(glm::cross(lookVec, upVec), lookVec)) * adjustedSpeed;
}

//Move the camera down
void CameraFPS::moveDown() {
	eyePos -= glm::normalize(glm::cross(glm::cross(lookVec, upVec), lookVec)) * adjustedSpeed;
}


//Update the adjusted speed of the camera movement using FPS
void CameraFPS::updateFPS(float FPS) { adjustedSpeed = moveSpeed / FPS;}

//Set the camera mouse movement sensitivity
void CameraFPS::setSensitivity(float cameraSensitivity) {
	sensitivity = cameraSensitivity;
}


//Rotate the camera to match the current cursor position
void CameraFPS::updateCursorPos(double xPos, double yPos, bool doRotation) {
	//Ignore the first cursor movement as it needs to be a reference point
	//Otherwise the camera will jump on the first input
	if (!hasLastCursor || !doRotation) {
		hasLastCursor = true;
	}
	else {
		//Get distance between this mouse position and the last
		float xDiff = xPos - lastCursorPos.x;
		float yDiff = lastCursorPos.y - yPos;

		//Apply the mouse sensitivity value
		xDiff *= sensitivity;
		yDiff *= sensitivity;

		//Add the difference to the camera rotations
		pitch += xDiff;
		yaw += yDiff;

		//Add limits to the yaw rotation (up and down)
		//Prevents upside-down camera
		if (yaw > 89.0f) { yaw = 89.0f; }
		if (yaw < -89.0f) { yaw = -89.0f; }
		
		//Move the look vector to the new rotation while maintaing its gradient
		glm::vec3 newTarget;
		newTarget.x = sin(glm::radians(pitch)) * cos(glm::radians(yaw));
		newTarget.y = sin(glm::radians(yaw));
		newTarget.z = cos(glm::radians(pitch)+ glm::radians(180.0f)) * cos(glm::radians(yaw));
		lookVec = glm::normalize(newTarget);
	}

	lastCursorPos = glm::vec2(xPos, yPos);
}

