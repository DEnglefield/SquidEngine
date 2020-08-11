

#include "SquidEngine.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "SampleShapes.h"
#include "Camera.h"
#include "CameraFPS.h"
#include "Texture.h"
#include "Voxel.h"

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

	MaterialList Materials;

	Voxel voxelArea(0,0,0);
	voxelArea.setSeed(427942);
	voxelArea.setSurfaceLevel(0.4f);
	voxelArea.setFrequency(1.0f);
	voxelArea.setOctaves(8.0f);
	voxelArea.outputNoise("debugImages/voxel.png",256,256);
	voxelArea.populatePoints(100, 50, 100);
	voxelArea.setScale(20.0f, 10.0f, 20.0f);
	voxelArea.build();

	while (!mainWindow.closing())
	{
		if (mainWindow.getKey(GLFW_KEY_PERIOD) == GLFW_PRESS) {
			voxelArea.setSurfaceLevel(voxelArea.getSurfaceLevel() + 0.05f);
			std::cout <<" Surface Level: " <<  voxelArea.getSurfaceLevel() << std::endl;
			voxelArea.build();
		}

		if (mainWindow.getKey(GLFW_KEY_COMMA) == GLFW_PRESS) {
			voxelArea.setSurfaceLevel(voxelArea.getSurfaceLevel() - 0.05f);
			std::cout << " Surface Level: " << voxelArea.getSurfaceLevel() << std::endl;
			voxelArea.build();
		}

		processCameraInput(mainWindow,cam);

		vector<ViewPort> viewList = viewsMain;

		for (int i = 0; i < viewList.size(); ++i) {
			viewList[i].use();

			cam.setView(viewList[i]);
			shader.setMat4("viewMatrix", cam.getViewMatrix());
			shader.setMat4("projMatrix", cam.getProjectionMatrix());

			shader.setMat4("worldMatrix", worldMatrix);
			shader.setVec3("cameraPos", cam.getPosition());
			shader.setVec3("lightPos", glm::vec3(10,10,10));

			glClearColor(0.43f, 0.71f, 0.86 - ((0.86f / views.size()) * (i)), 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.setMat4("modelMatrix", voxelArea.getModelMatrix());
			shader.setVec4("material.colour", voxelArea.getColour());
			Material mat = Materials.obsidian;
			shader.setVec3("material.ambient", mat.ambient);
			shader.setVec3("material.diffuse", mat.diffuse);
			shader.setVec3("material.specular", mat.specular);
			shader.setFloat("material.reflectivity", mat.highlight);
			voxelArea.draw();
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

