
#include "Camera.h"


//Create a camera using eye and look target vector
Camera::Camera(float eyeX, float eyeY, float eyeZ,
	float lookX, float lookY, float lookZ) {

	setPerspective(CAMERA_PERSPECTIVE);
	upVec = glm::vec3(0, 1, 0);

	setPosition(eyeX, eyeY, eyeZ);
	lookAt(lookX, lookY, lookZ);
	setProjection(viewWidth, viewHeight, minRender, maxRender);
}

//Create a camera using eye position and look/target vector as vectors
Camera::Camera(glm::vec3 eye, glm::vec3 look) {
	setPerspective(CAMERA_PERSPECTIVE);
	upVec = glm::vec3(0, 1, 0);
	
	setPosition(eye.x, eye.y, eye.z);
	lookAt(look.x, look.y, look.z);
	setProjection(viewWidth, viewHeight, minRender, maxRender);
}

//Set the type of projection being CAMERA_ORTHOGRAPHIC or CAMERA_PERSPECTIVE
void Camera::setPerspective(int viewType) { viewPerspective = viewType; }

//Set the field of view for the camera and update the projection matrix
void Camera::setFOV(float FOV) {
	fieldOfView = FOV;
	setProjection(viewWidth, viewHeight, minRender, maxRender);
}

//Update the with and height of the viewport used by this camera
//Will update projection matrix
void Camera::setView(int width, int height) {
	viewWidth = width;
	viewHeight = height;
	setProjection(viewWidth, viewHeight, minRender, maxRender);
}

//Get the view size from a ViewPort object
void Camera::setView(ViewPort& viewPort) {
	getViewPortSize(viewPort, &viewWidth, &viewHeight);
	setProjection(viewWidth, viewHeight, minRender, maxRender);
}

//Update the maximum and minimum render distance and update projection matrix
void Camera::setRenderDistance(float near, float far) {
	minRender = near;
	maxRender = far;
	setProjection(viewWidth, viewHeight, minRender, maxRender);
}

//Update the whole projection matrix with the desired perspective type (excluding FOV)
void Camera::setProjection(int viewWidth, int viewHeight, float near, float far) {
	if (viewPerspective == CAMERA_PERSPECTIVE) {
		projMatrix = glm::perspective(glm::radians(fieldOfView), (float)viewWidth / (float)viewHeight, near, far);
	}
	else if (viewPerspective == CAMERA_ORTHOGRAPHIC) {
		projMatrix = glm::ortho(0.0f, (float)viewWidth, (float)viewHeight, 0.0f, near,far);
	}
}

//Set the eye/camera position and update the view matrix 
void Camera::setPosition(float eyeX, float eyeY, float eyeZ) {
	eyePos = glm::vec3(eyeX, eyeY, eyeZ);
	viewMatrix = glm::lookAt(eyePos, lookVec, upVec);
}

//Set the look vector for the camera (idealy normalised)
//Also updates the view matrix
void Camera::lookAt(float lookX, float lookY, float lookZ) {
	lookVec = glm::vec3(lookX, lookY, lookZ);
	viewMatrix = glm::lookAt(eyePos, lookVec, upVec);
}
