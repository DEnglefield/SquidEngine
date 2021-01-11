
#include "Camera.h"
#include <iostream>


//Create a camera using eye and look target vector
Camera::Camera(float eyeX, float eyeY, float eyeZ,
	float lookX, float lookY, float lookZ) {

	setPerspective(CAMERA_PERSPECTIVE);
	upVec = glm::vec3(0, 1, 0);

	setPosition(eyeX, eyeY, eyeZ);
	lookAt(lookX, lookY, lookZ);
}

//Create a camera using eye position and look/target vector as vectors
Camera::Camera(glm::vec3 eye, glm::vec3 look) {
	setPerspective(CAMERA_PERSPECTIVE);
	upVec = glm::vec3(0, 1, 0);
	
	setPosition(eye.x, eye.y, eye.z);
	lookAt(look.x, look.y, look.z);
}


//Set camera projection type e.g. CAMERA_ORTHOGRAPHIC
void Camera::setPerspective(int type) { viewPerspective = type; }
//Set scale for orthographic clipping space
void Camera::setClippingSize(float clipScale) {clippingSize = clipScale;}
//Set the field of view for the camera
void Camera::setFOV(float FOV) { fieldOfView = FOV; }
//Update the with and height of the viewport used by this camera
void Camera::setView(int width, int height) { viewWidth = width; viewHeight = height; }
//Get the view size from a ViewPort object
void Camera::setView(ViewPort& viewPort) { setView(viewPort.width, viewPort.height); }
//Update the maximum and minimum render distance
void Camera::setRenderDistance(float near, float far) { minRender = near; maxRender = far; }


//Update current projection matrix
void Camera::updateProjection() {
	if (viewPerspective == CAMERA_PERSPECTIVE) { 
		projMatrix = glm::perspective(glm::radians(fieldOfView), (float)viewWidth / (float)viewHeight, minRender, maxRender); 
	}

	if (viewPerspective == CAMERA_ORTHOGRAPHIC) { 
		projMatrix = glm::ortho(-clippingSize, clippingSize, -clippingSize, clippingSize, minRender, maxRender);
	}
}

//Set the eye/camera position 
void Camera::setPosition(float eyeX, float eyeY, float eyeZ) {
	eyePos = glm::vec3(eyeX, eyeY, eyeZ);
}

//Set the look vector for the camera (idealy normalised)
void Camera::lookAt(float lookX, float lookY, float lookZ) {
	lookVec = glm::vec3(lookX, lookY, lookZ);
}

//Update and return the view matrix
glm::mat4 Camera::getViewMatrix() {
	viewMatrix = glm::lookAt(eyePos, eyePos + lookVec, upVec);
	return viewMatrix;
}

//Update and return the projection matrix
glm::mat4 Camera::getProjectionMatrix() {
	updateProjection();
	return projMatrix;
}

//Get the camera eye position
glm::vec3 Camera::getPosition() { return eyePos; }