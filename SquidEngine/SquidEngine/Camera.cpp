
#include "Camera.h"


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


//Set this camera as the active camera
void Camera::use(ShaderProgram& shader) {
	shader.setCameraPosition(eyePos);
	shader.setMat4(VIEW_MATRIX_UNIFORM, getViewMatrix());
	shader.setMat4(PROJECTION_MATRIX_UNIFORM, getProjectionMatrix());
}


//Set the type of projection being CAMERA_ORTHOGRAPHIC or CAMERA_PERSPECTIVE
void Camera::setPerspective(int viewType) { viewPerspective = viewType; }
//Set the field of view for the camera
void Camera::setFOV(float FOV) { fieldOfView = FOV; }
//Update the with and height of the viewport used by this camera
void Camera::setView(int width, int height) { viewWidth = width; viewHeight = height; }
//Get the view size from a ViewPort object
void Camera::setView(ViewPort& viewPort) { getViewPortSize(viewPort, &viewWidth, &viewHeight); }
//Update the maximum and minimum render distance
void Camera::setRenderDistance(float near, float far) { minRender = near; maxRender = far; }


//Update the whole projection matrix with the desired perspective type (excluding FOV)
void Camera::setProjection(int viewWidth, int viewHeight, float near, float far) {
	if (viewPerspective == CAMERA_PERSPECTIVE) {
		projMatrix = glm::perspective(glm::radians(fieldOfView), (float)viewWidth / (float)viewHeight, near, far);
	}
	else if (viewPerspective == CAMERA_ORTHOGRAPHIC) {
		projMatrix = glm::ortho(0.0f, (float)viewWidth, (float)viewHeight, 0.0f, near,far);
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
	setProjection(viewWidth, viewHeight, minRender, maxRender);
	return projMatrix;
}

//Get the camera eye position
glm::vec3 Camera::getPosition() { return eyePos; }