#pragma once

#include "SquidEngine.h"
#include "ShaderProgram.h"
#include "Window.h"

#define CAMERA_PERSPECTIVE 1
#define CAMERA_ORTHOGRAPHIC 2

//A camera used to view the contents of the scene with the chosen properties
class Camera {
private:
	int viewPerspective;
protected:
	int viewWidth = 800;
	int viewHeight = 600;
public:
	glm::vec3 eyePos;
	glm::vec3 lookVec;
	glm::vec3 upVec;

	float minRender = 0.01f;
	float maxRender = 100.0f;
	float fieldOfView = 45;

	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;

	//Create a camera using eye and look target vector
	Camera(float eyeX, float eyeY, float eyeZ,
		float lookX, float lookY, float lookZ);
	Camera(glm::vec3 eye, glm::vec3 look);

	//Set the type of projection being CAMERA_ORTHOGRAPHIC or CAMERA_PERSPECTIVE
	void setPerspective(int viewType);
	
	//Set the field of view for the camera and update the projection matrix
	void setFOV(float FOV);
	//Update the with and height of the viewport used by this camera
	//Will update projection matrix
	void setView(int width, int height);
	//Get the view size from a ViewPort object
	void setView(ViewPort& viewPort);
	//Update the maximum and minimum render distance and update projection matrix
	void setRenderDistance(float near, float far);
	//Update the whole projection matrix
	void setProjection(int viewWidth, int viewHeight, float near, float far);

	//Set the eye/camera position and update the view matrix 
	void setPosition(float eyeX, float eyeY, float eyeZ);
	//Set the look vector for the camera (idealy normalised)
	//Also updates the view matrix
	void lookAt(float lookX, float lookY, float lookZ);
};