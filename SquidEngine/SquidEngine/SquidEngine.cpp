

#include "SquidEngine.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Cube.h"
#include "Camera.h"
#include "CameraFPS.h"
#include "Texture.h"
#include "Voxel.h"
#include "Model.h"

using namespace std;

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

	ShaderProgram basicShader("Shaders/BasicShader/BasicShader.vert", "Shaders/BasicShader/BasicShader.frag");
	ShaderProgram lightingShader("Shaders/LightingShader/LightingShader.vert", "Shaders/LightingShader/LightingShader.frag");

	ShaderProgram shader = lightingShader;
	shader.createShaderProgram();
	shader.use();

	Texture defaultTex();

	Texture moon("Resources/Textures/moon.jpg");
	Texture star("Resources/Textures/star.png");
	Texture cubeMap("Resources/Textures/floor.bmp");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	MaterialList Materials;

	/*
	Voxel voxelArea(0,0,0);
	voxelArea.setMaterial(Materials.obsidian);
	voxelArea.setSeed(427942);
	voxelArea.setSurfaceLevel(0.4f);
	voxelArea.setFrequency(1.0f);
	voxelArea.setOctaves(8.0f);
	voxelArea.outputNoise("debugImages/voxel.png",256,256);
	voxelArea.populatePoints(100, 50, 100);
	voxelArea.setScale(20.0f, 10.0f, 20.0f);
	voxelArea.build();
	*/

	glm::mat4 worldMatrix(1.0f);
	shader.setMat4(WORLD_MATRIX_UNIFORM, worldMatrix);

	PointLight orbitLightX(0, -1, 0);
	unsigned int orbitLightXHandle = shader.addPointLight(orbitLightX);

	PointLight orbitLightZ(1.25f, 0, 0);
	unsigned int orbitLightZHandle = shader.addPointLight(orbitLightZ);

	PointLight orbitLightXZ(0, 1.5f, 0);
	unsigned int orbitLightXZHandle = shader.addPointLight(orbitLightXZ);

	glm::mat4 lightOrbitMatrixX(1.0f);
	lightOrbitMatrixX = glm::rotate(lightOrbitMatrixX, glm::radians(2.0f), glm::vec3(1, 0, 0));

	glm::mat4 lightOrbitMatrixZ(1.0f);
	lightOrbitMatrixZ = glm::rotate(lightOrbitMatrixZ, glm::radians(2.0f), glm::vec3(0, 0, 1));

	glm::mat4 lightOrbitMatrixXZ(1.0f);
	lightOrbitMatrixXZ = glm::rotate(lightOrbitMatrixXZ, glm::radians(2.0f), glm::vec3(1, 0, 1));

	Cube orbitCubeX(orbitLightX.position.x, orbitLightX.position.y, orbitLightX.position.z);
	Cube orbitCubeZ(orbitLightZ.position.x, orbitLightZ.position.y, orbitLightZ.position.z);
	Cube orbitCubeXZ(orbitLightXZ.position.x, orbitLightXZ.position.y, orbitLightXZ.position.z);
	orbitCubeX.setScale(0.25f, 0.25f, 0.25f);
	orbitCubeZ.setScale(0.25f, 0.25f, 0.25f);
	orbitCubeXZ.setScale(0.25f, 0.25f, 0.25f);
	orbitCubeX.setMaterial(Materials.bronze);
	orbitCubeZ.setMaterial(Materials.bronze);
	orbitCubeXZ.setMaterial(Materials.bronze);

	Cube testCube(0.0f, 0.0f, 0.0f);
	testCube.setMaterial(Materials.copper);
	testCube.addTexture(star.getID());

	Model spaceShip(0,0,0,"C:/Users/dalet/OneDrive/Desktop/SpaceShip.obj");

	double frameStart = glfwGetTime();
	while (!mainWindow.closing())
	{

		processCameraInput(mainWindow,cam);

		vector<ViewPort> viewList = viewsMain;

		for (int i = 0; i < viewList.size(); ++i) {
			viewList[i].use();
			cam.setView(viewList[i]);

			glClearColor(0.43f, 0.71f, 0.86 - ((0.86f / views.size()) * (i)), 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//voxelArea.draw(shader);
			//testCube.draw(shader);
			spaceShip.draw(shader);
		}

		orbitLightX.position = (lightOrbitMatrixX * glm::vec4(orbitLightX.position,1.0f));
		shader.setVec3(shader.getLightPropertyName(POINT_LIGHT_UNIFORM,LIGHT_POSITION_UNIFORM,orbitLightXHandle).c_str(), orbitLightX.position);
		orbitCubeX.setPosition(orbitLightX.position.x, orbitLightX.position.y, orbitLightX.position.z);

		orbitLightZ.position = (lightOrbitMatrixZ * glm::vec4(orbitLightZ.position, 1.0f));
		shader.setVec3(shader.getLightPropertyName(POINT_LIGHT_UNIFORM, LIGHT_POSITION_UNIFORM, orbitLightZHandle).c_str(), orbitLightZ.position);
		orbitCubeZ.setPosition(orbitLightZ.position.x, orbitLightZ.position.y, orbitLightZ.position.z);

		orbitLightXZ.position = (lightOrbitMatrixXZ * glm::vec4(orbitLightXZ.position, 1.0f));
		shader.setVec3(shader.getLightPropertyName(POINT_LIGHT_UNIFORM, LIGHT_POSITION_UNIFORM, orbitLightXZHandle).c_str(), orbitLightXZ.position);
		orbitCubeXZ.setPosition(orbitLightXZ.position.x, orbitLightXZ.position.y, orbitLightXZ.position.z);

		//orbitCubeX.draw(shader);
		//orbitCubeZ.draw(shader);
		//orbitCubeXZ.draw(shader);
		

		cam.use(shader);

		glfwSwapInterval(1);
		glfwSwapBuffers(glfwGetCurrentContext());
		glfwPollEvents();

		cam.updateFPS(1 / (glfwGetTime() - frameStart));
		frameStart = glfwGetTime();
	}	

	return 0;
}

