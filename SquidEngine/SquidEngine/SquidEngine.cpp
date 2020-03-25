

#include "SquidEngine.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "SampleShapes.h"
#include "Camera.h"


using namespace std;


glm::mat4 worldMatrix(1.0f);
glm::mat4 viewMatrix(1.0f);
glm::mat4 projMatrix(1.0f);

void mainResizeEvent(GLFWwindow* window, int width, int height)
{
	glViewport(0,0,width,height);
	cout << "Resized to :" << width << ", " << height << endl;
	projMatrix = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.01f, 100.0f);
}


void processInput(Window& window)
{
	if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		window.close();
}



int main()
{
	cout << "Hello World" << endl;

	Window mainWindow(800, 600, "SquidEngine");

	mainWindow.show();
	mainWindow.setResizeEvent(mainResizeEvent);
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

	ShaderProgram shader("VertexShader.vert", nullptr, "FragmentShader.frag");

	Square square(0,0,0);
	square.build();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	shader.use();
	shader.setMat4("worldMatrix", worldMatrix);

	shader.setVec4("col", glm::vec4(0.3, 0.5, 0.2, 1));

	Camera cam(0,0,3,0,0,-1);

	shader.setMat4("viewMatrix",cam.viewMatrix);
	
	while (!mainWindow.closing())
	{
		processInput(mainWindow);

		vector<ViewPort> viewList = views;

		for (int i = 0; i < viewList.size(); ++i) {
			viewList[i].use();

			cam.setView(viewList[i]);
			shader.setMat4("projMatrix", cam.projMatrix);

			glClearColor(0.43f, 0.71f, (1.0f / views.size()) * (i+1), 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			square.draw(shader);
		}


		glfwSwapBuffers(glfwGetCurrentContext());
		glfwPollEvents();
	}	

	return 0;
}

