

#include "SquidEngine.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Cube.h"
#include "Camera.h"
#include "CameraFPS.h"
#include "Texture.h"
#include "Voxel.h"
#include "Model.h"
#include "Materials.h"
#include "FrameBuffer.h"
#include "ScreenQuad.h"

using namespace std;

CameraFPS cam(0, 0, 3, 0, 0, -1);

void mainResizeEvent(GLFWwindow* window, int width, int height)
{
	cout << "Resized to :" << width << ", " << height << endl;
	resizeFrameBuffers(width, height);
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


	Materials materials;

	ViewPort viewMain(0.0f, 0.0f, 1.0f, 1.0f);
	ViewPort debugViewLeft(0.0f, 0.0f, 0.5f, 1.0f);
	ViewPort debugViewRight(0.5f, 0.0f, 0.5f, 1.0f);
	ViewPort view1(0.0f, 0.5f,0.5f, 0.5f);
	ViewPort view2(0.5f, 0.5f, 0.5f, 0.5f);
	ViewPort view3(0.0f, 0.0f, 0.5f, 0.5f);
	ViewPort view4(0.5f, 0.0f, 0.5f, 0.5f);

	vector<ViewPort> viewsMain = { viewMain };
	vector<ViewPort> debugViews = { debugViewLeft, debugViewRight };
	vector<ViewPort> views = { view1, view2, view3, view4 };

	glm::mat4 worldMatrix(1.0f);

	ShaderProgram basicShader("Shaders/BasicShader/BasicShader.vert", "Shaders/BasicShader/BasicShader.frag");
	ShaderProgram lightingShader("Shaders/LightingShader/LightingShader.vert", "Shaders/LightingShader/LightingShader.frag");
	ShaderProgram depthShader("Shaders/DepthShader/DepthShader.vert", "Shaders/DepthShader/DepthShader.frag");
	ShaderProgram quadShader("Shaders/QuadShader/QuadShader.vert", "Shaders/QuadShader/QuadShader.frag");

	quadShader.createShaderProgram();

	basicShader.createShaderProgram();
	basicShader.use();
	basicShader.setMat4(WORLD_MATRIX_UNIFORM, worldMatrix);

	depthShader.createShaderProgram();
	depthShader.use();
	depthShader.setMat4(WORLD_MATRIX_UNIFORM, worldMatrix);

	lightingShader.createShaderProgram();
	lightingShader.use();
	lightingShader.setMat4(WORLD_MATRIX_UNIFORM, worldMatrix);
	
	ShaderProgram shader = lightingShader;
	shader.use();

	Texture star("Resources/Textures/star.png");
	Texture starSpecular("Resources/Textures/starSpecular.bmp");
	Texture cubeMap("Resources/Textures/floor.bmp");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	DirectionalLight sceneLight(-0.25f,-0.25f, 0.0f);
	//sceneLight.ambient = glm::vec3(0.7f, 0.7f, 0.7f);
	//sceneLight.diffuse = glm::vec3(0,0,0);
	//sceneLight.specular = glm::vec3(0.9f,0.9f,0.9f);
	unsigned int sceneLightHandle = shader.addDirectionalLight(sceneLight);

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
	orbitCubeX.setMaterial(materials.pearl);
	orbitCubeZ.setMaterial(materials.pearl);
	orbitCubeXZ.setMaterial(materials.pearl);

	Material starMaterial(star, starSpecular, 32);

	Cube testCube(0.0f, 0.0f, 0.0f);
	testCube.setMaterial(starMaterial);

	defaultTextureID = 0;

	//Model spaceShip(0,0,0,"C:/Users/dalet/OneDrive/Desktop/SpaceShip.obj");
	//Model moon(0, 0, 0, "Resources/Models/MoonModel/Moon 2k.obj");
	
	Model building(0, 0, 0, "Resources/Models/Buildings/Residential Buildings 001.obj");
	building.setScale(0.5f, 0.5f, 0.5f);
	Model building2(0, 0, 0, "Resources/Models/Buildings/Residential Buildings 005.obj");
	building2.setScale(0.5f, 0.5f, 0.5f);
	building2.setPosition(0, 0, 14);

	//Model room(0, 0, 0, "Resources/Models/RoomModel/interior.obj");
	//room.setScale(0.01f, 0.01f, 0.01f);

	ScreenQuad screenQuad(0.1f, 0.1f, 0.9f, 0.9f);

	vector<ViewPort> viewList = debugViews;//viewsMain;

	//glm::vec3 firstClearColour = glm::vec3(1, 0, 0);
	FrameBuffer debugViewBuffers[] = {
		FrameBuffer(glfwGetCurrentContext()),
		FrameBuffer(glfwGetCurrentContext())
	};

	ShaderProgram debugShaders[] = {
		lightingShader,
		basicShader
	};

	DrawFrameBuffer outputFrameBuffer;
	outputFrameBuffer.setClearColour(glm::vec3(0,1,0));


	double frameStart = glfwGetTime();
	while (!mainWindow.closing()) {
		processCameraInput(mainWindow,cam);

		orbitLightX.position = (lightOrbitMatrixX * glm::vec4(orbitLightX.position, 1.0f));
		orbitCubeX.setPosition(orbitLightX.position.x, orbitLightX.position.y, orbitLightX.position.z);

		orbitLightZ.position = (lightOrbitMatrixZ * glm::vec4(orbitLightZ.position, 1.0f));
		orbitCubeZ.setPosition(orbitLightZ.position.x, orbitLightZ.position.y, orbitLightZ.position.z);

		orbitLightXZ.position = (lightOrbitMatrixXZ * glm::vec4(orbitLightXZ.position, 1.0f));
		orbitCubeXZ.setPosition(orbitLightXZ.position.x, orbitLightXZ.position.y, orbitLightXZ.position.z);


		//Pass 1 & 2
		for (int i = 0; i < viewList.size(); ++i) {
			debugViewBuffers[i].use(viewMain);
			cam.setView(viewMain);

			shader = debugShaders[i];
			shader.use();
			cam.use(shader);

			shader.setVec3(shader.getLightPropertyName(POINT_LIGHT_UNIFORM, LIGHT_POSITION_UNIFORM, orbitLightXHandle).c_str(), orbitLightX.position);
			shader.setVec3(shader.getLightPropertyName(POINT_LIGHT_UNIFORM, LIGHT_POSITION_UNIFORM, orbitLightZHandle).c_str(), orbitLightZ.position);
			shader.setVec3(shader.getLightPropertyName(POINT_LIGHT_UNIFORM, LIGHT_POSITION_UNIFORM, orbitLightXZHandle).c_str(), orbitLightXZ.position);

			orbitCubeX.draw(shader);
			orbitCubeZ.draw(shader);
			orbitCubeXZ.draw(shader);
			testCube.draw(shader);

			building.draw(shader);
			building2.draw(shader);
		}
		
		//Pass 3 to show pass 1 and 2
		for (int i = 0; i < viewList.size(); ++i) {
			
			outputFrameBuffer.use(viewList[i]);
			
			screenQuad.setTexture(debugViewBuffers[i].getTextureOutput());
			//screenQuad.setTexture(star.getID());
			quadShader.use();
			screenQuad.draw();

		}
		
		

		glfwSwapInterval(1);
		glfwSwapBuffers(glfwGetCurrentContext());
		glfwPollEvents();

		cam.updateFPS(1 / (glfwGetTime() - frameStart));
		frameStart = glfwGetTime();
	}	

	return 0;
}

