#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	float clippingSize = 1.0f;

	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;

	//Update the currently selected projection matrix with current parameters
	void updateProjection();

public:
	//Camera position and target vectors
	glm::vec3 eyePos;
	glm::vec3 lookVec;
	glm::vec3 upVec;

	//Camera properties
	float minRender = 0.01f;
	float maxRender = 100.0f;
	float fieldOfView = 45;

	//Create a camera using eye and look target vector
	Camera(float eyeX, float eyeY, float eyeZ,
		float lookX, float lookY, float lookZ);
	Camera(glm::vec3 eye, glm::vec3 look);


	//Set camera projection type e.g. CAMERA_ORTHOGRAPHIC
	void setPerspective(int type);
	
	//Set the field of view for the camera
	void setFOV(float FOV);
	//Set scale for orthographic clipping space
	void setClippingSize(float clippingScale);
	//Update the with and height of the viewport used by this camera
	//Will update projection matrix
	void setView(int width, int height);
	//Get the view size from a ViewPort object
	void setView(ViewPort& viewPort);
	//Update the maximum and minimum render distance
	void setRenderDistance(float near, float far);
	//Update the whole projection matrix
	void setProjection(int viewWidth, int viewHeight, float near, float far);

	//Set the eye/camera position 
	void setPosition(float eyeX, float eyeY, float eyeZ);
	//Set the look vector for the camera (idealy normalised)
	void lookAt(float lookX, float lookY, float lookZ);

	//Update and return the view matrix
	glm::mat4 getViewMatrix();
	//Update and return the projection matrix
	glm::mat4 getProjectionMatrix();

	//Get the camera eye position
	glm::vec3 getPosition();
};