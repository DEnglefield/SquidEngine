
#include "App.h"
#include "SquidEngine.h"
#include "Cube.h"
#include "CameraFPS.h"
#include "BasicLightingShader.h"
#include <iostream>
#include "Model.h"
#include "Light.h"
#include "UniformBuffer.h"

Cube* lightCube;
Cube* testCube;
Cube* ground;
Model* building;
Model* bauble;
Material* testMat;
Material* lightMat;
Material* groundMat;
SkyboxLightingShader* lightingShader;
SkyBox* skybox;
DirectionalLight dirLight(0.0f,0.0f,-1.0f);
CameraFPS cam(0, 0, 3, 0, 0, -1);
Model* cottage;

glm::mat4 lightRotator(1.0f);
glm::vec3 lightDir1(0, 0, 0);
glm::vec3 lightDir2(0, 0, 0);

//Called on engine initialisation
void Game::onInit() {
	//cam.setRenderDistance(0.01f,100.0f);
	cam.setRenderDistance(1.0f, 7.5f);

	//Create and load scene shapes
	testCube = new Cube(0,0,0);
	ground = new Cube(0, -1, 0);
	lightCube = new Cube(0,0,0);
	lightCube->setScale(0.25f,0.25f,0.25f);
	ground->setScale(100,1,100);
	building = new Model(-5,1.6f,5,"Resources/Models/Buildings/Residential Buildings 001.obj");

	//Load skybox images
	std::vector<std::string> skyboxImages = {
		"Resources/Textures/skybox/right.jpg",
		"Resources/Textures/skybox/left.jpg",
		"Resources/Textures/skybox/top.jpg",
		"Resources/Textures/skybox/bottom.jpg",
		"Resources/Textures/skybox/front.jpg",
		"Resources/Textures/skybox/back.jpg"
	};

	skybox = new SkyBox(skyboxImages);

	//Create scene materials
	testMat = new Material(
		"Resources/Textures/star.png", 
		"Resources/Textures/starSpecular.bmp", 
		64);

	groundMat = new Material(glm::vec3(0.5f, 0.9f, 0.5f), glm::vec3(0.45f, 0.8f, 0.45f), 32);
	groundMat->reflectivity = 0;

	lightMat = new Material(
		"Resources/Textures/square.png",
		"Resources/Textures/square.png",
		64);

	ground->setMaterial(*groundMat);
	testCube->setMaterial(*testMat);
	lightCube->setMaterial(*lightMat);
	

	//Create scene lighting shader
	lightingShader = new SkyboxLightingShader();
	lightingShader->onInit();
	lightingShader->setMainCamera(cam);

	//Add shapes to created lighting shader
	lightingShader->addShape(*testCube);
	lightingShader->addModel(*building);
	lightingShader->addShape(*ground);
	//lightingShader->addShape(*lightCube);

	//Add directional light and skybox to shader
	lightingShader->addDirectionalLight(dirLight);
	lightingShader->setSkybox(*skybox);

	//Pre-compute rotation matrix to rotate directional light
	lightRotator = glm::rotate(lightRotator, 0.01f, glm::vec3(0, 0, 1));
	lightDir1 = dirLight.direction;
	lightDir2 = -dirLight.direction;
}


//Called on each draw call
//Implements real-time scene transformations
void Game::onDraw(double deltaTime, int viewID, ViewPort& view) {
	cam.updateFrameTime(deltaTime);
	cam.setView(view);

	//Scale light rotation (day/night cycle) with current frame time
	float angle = 0.75f * (float)deltaTime;
	lightRotator = glm::mat4(1.0f);
	lightRotator = glm::rotate(lightRotator, angle, glm::vec3(0, 0, 1));

	//Rotate light clockwise on first screen partition
	if (viewID == 0) {
		//lightDir1 = glm::vec4(lightDir1, 1) * lightRotator;
		//dirLight.direction = lightDir1;
		lightCube->setPosition(0,0.0f,3);
	}
	//Rotate light anti-clockwise on second screen partition
	else {
		lightDir2 = glm::vec4(lightDir2, 1) * lightRotator;
		dirLight.direction = lightDir2;
	}


}


//Called on input check event
void Game::onInputCheck(GLFWwindow* window) {
	//Test camera control keys
	if (glfwGetKey(window, GLFW_KEY_W) == true) { cam.moveForward(); }
	if (glfwGetKey(window, GLFW_KEY_S) == true) { cam.moveBackward(); }
	if (glfwGetKey(window, GLFW_KEY_A) == true) { cam.moveLeft(); }
	if (glfwGetKey(window, GLFW_KEY_D) == true) { cam.moveRight(); }
	if (glfwGetKey(window, GLFW_KEY_C) == true) { cam.moveDown(); }
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == true) { cam.moveDown(); }
	if (glfwGetKey(window, GLFW_KEY_SPACE) == true) { cam.moveUp(); }
}


//Called on mosue move event
void Game::onMouseMove(int mouseX, int mouseY) {
	//Update camera's known mouse position
	cam.updateCursorPos(mouseX, mouseY, isWindowFocused());
}


//Called on scroll wheel moved
void Game::onMouseScroll(double scrollX, double scrollY) {
	//Adjust base camera movement speed
	cam.setMoveSpeed(cam.getMoveSpeed() + (scrollY*0.5f));
}

//Called on window and framebuffer resize
void Game::onWindowResize(int width, int height) { 
	//Update camera with new render area
	cam.setView(width,height);
}


//Called when render window is closed
void Game::onClose() {
	testCube->destroy();
	testMat->destroy();
	ground->destroy();
	groundMat->destroy();
	building->destroy();
	lightingShader->destroy();
	delete testCube;
	delete testMat;
	delete ground;
	delete groundMat;
	delete building;
	delete lightingShader;
}





int main() {
	//Create and run game engine instance
	Game gameInstance;
	createEngineWindow(800, 600, "SquidEngine");
	runEngine(gameInstance);

	return 0;
}