

#include "SquidEngine.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "SampleShapes.h"
#include "Camera.h"
#include "CameraFPS.h"
#include "Texture.h"

using namespace std;

glm::mat4 worldMatrix(1.0f);
glm::mat4 viewMatrix(1.0f);
glm::mat4 projMatrix(1.0f);


CameraFPS cam(0, 0, 3, 0, 0, -1);

void mainResizeEvent(GLFWwindow* window, int width, int height)
{
	cout << "Resized to :" << width << ", " << height << endl;
}



void cursorCallBack(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		cam.updateCursorPos(xpos, ypos, true);
	}
	else {
		cam.updateCursorPos(xpos, ypos, false);
	}
	
}

void scrollCallBack(GLFWwindow* window, double scrollX, double scrollY) {

	if (cam.moveSpeed + scrollY < 0) {
		cam.moveSpeed = 0;
	}
	else {
		cam.moveSpeed += scrollY;
	}
	
}

void mouseBtnCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (glfwGetWindowAttrib(window, GLFW_HOVERED)) {
		if (button == GLFW_MOUSE_BUTTON_1) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}

void processCameraInput(Window& window, CameraFPS& cam) //Cant get current context with wrapper
{
	if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetInputMode(window.form, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (window.getKey(GLFW_KEY_W) == GLFW_PRESS) {
		cam.moveForward();
	}

	if (window.getKey(GLFW_KEY_S) == GLFW_PRESS) {
		cam.moveBackward();
	}

	if (window.getKey(GLFW_KEY_A) == GLFW_PRESS) {
		cam.moveLeft();
	}

	if (window.getKey(GLFW_KEY_D) == GLFW_PRESS) {
		cam.moveRight();
	}

	if (window.getKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
		cam.moveUp();
	}

	if (window.getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cam.moveDown();
	}
}



int main()
{
	cout << "Hello World" << endl;

	Window mainWindow(800, 600, "SquidEngine");

	mainWindow.show();
	glfwSetFramebufferSizeCallback(mainWindow.form, mainResizeEvent);
	glfwSetCursorPosCallback(mainWindow.form, cursorCallBack);
	glfwSetMouseButtonCallback(mainWindow.form, mouseBtnCallBack);
	glfwSetScrollCallback(mainWindow.form, scrollCallBack);
	mainWindow.setActive();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	ViewPort viewMain(0.0f, 0.0f, 1.0f, 1.0f);
	ViewPort view1(0.0f, 0.5f,0.5f, 0.5f);
	ViewPort view2(0.5f, 0.5f, 0.5f, 0.5f);
	ViewPort view3(0.0f, 0.0f, 0.5f, 0.5f);
	ViewPort view4(0.5f, 0.0f, 0.5f, 0.5f);

	vector<ViewPort> viewsMain = { viewMain };
	vector<ViewPort> views = { view1, view2, view3, view4 };

	ShaderProgram basicShader("Shaders/BasicShader/BasicShader.vert", nullptr, "Shaders/BasicShader/BasicShader.frag");
	ShaderProgram lightingShader("Shaders/LightingShader/LightingShader.vert", nullptr, "Shaders/LightingShader/LightingShader.frag");

	ShaderProgram shader = lightingShader;

	Texture moon("Resources/Textures/moon.jpg");
	Texture star("Resources/Textures/star.png");
	Texture cubeMap("Resources/Textures/floor.bmp");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);


	shader.use();
	shader.setMat4("worldMatrix", worldMatrix);

	shader.setVec4("col", glm::vec4(0.3, 0.5, 0.2, 1));

	double frameStart = glfwGetTime();;
	float FPS = 60;



	while (!mainWindow.closing())
	{
		processCameraInput(mainWindow,cam);

		vector<ViewPort> viewList = views;

		for (int i = 0; i < viewList.size(); ++i) {
			viewList[i].use();

			cam.setView(viewList[i]);
			shader.setMat4("viewMatrix", cam.getViewMatrix());
			shader.setMat4("projMatrix", cam.getProjectionMatrix());

			shader.setMat4("worldMatrix", worldMatrix);
			shader.setVec3("cameraPos", cam.getPosition());
			shader.setVec3("lightPos", glm::vec3(0,5,0));

			glClearColor(0.43f, 0.71f, 0.86 - ((0.86f / views.size()) * (i)), 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			int terrainSize = 12;

			for (int terrainX = 0; terrainX < terrainSize; ++terrainX) {
				for (int terrainZ = 0; terrainZ < terrainSize; ++terrainZ) {
					Cube terrainCube(terrainX-5, -1, terrainZ-5);
					terrainCube.build();
					terrainCube.addTexture(cubeMap.getID());

					shader.setMat4("modelMatrix", terrainCube.getModelMatrix());
					shader.setVec4("material.colour", terrainCube.getColour());
					if (i == 0) {
						//Green Rubber
						shader.setVec3("material.ambient", glm::vec3(0.0f, 0.05f, 0.0f));
						shader.setVec3("material.diffuse", glm::vec3(0.4f, 0.5f, 0.4f));
						shader.setVec3("material.specular", glm::vec3(0.04f, 0.7f, 0.04f));
						shader.setFloat("material.reflectivity", 10);
						
					}

					if (i == 1) {
						//Chrome
						shader.setVec3("material.ambient", glm::vec3(0.25f, 0.25f, 0.25f));
						shader.setVec3("material.diffuse", glm::vec3(0.4f,0.4f,0.4f));
						shader.setVec3("material.specular", glm::vec3(0.774597f, 0.774597f, 0.774597f));
						shader.setFloat("material.reflectivity", 78.0f);
					}


					if (i == 2) {
						//Emerald
						shader.setVec3("material.ambient", glm::vec3(0.0215f, 0.1745f, 0.0215f));
						shader.setVec3("material.diffuse", glm::vec3(0.07568f, 0.61424f, 0.07568f));
						shader.setVec3("material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
						shader.setFloat("material.reflectivity", 78.0f);
					}

					if (i == 3) {
						//Red Plastic
						shader.setVec3("material.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
						shader.setVec3("material.diffuse", glm::vec3(0.5f, 0.0f, 0.0f));
						shader.setVec3("material.specular", glm::vec3(0.7f, 0.6f, 0.6f));
						shader.setFloat("material.reflectivity", 32.0f);
					}
					
					terrainCube.draw();

				}
			}

			Cube square(0, 0, 0);
			square.build();
			square.addTexture(star.getID());

		}

		cam.updateFPS(FPS);

		glfwSwapInterval(1);
		glfwSwapBuffers(glfwGetCurrentContext());
		glfwPollEvents();

		FPS = 1 / (glfwGetTime() - frameStart);
		
		//std::cout << "FPS: " << FPS << " Frame Time: " << (glfwGetTime() - frameStart) << std::endl;
		frameStart = glfwGetTime();
	}	

	return 0;
}

