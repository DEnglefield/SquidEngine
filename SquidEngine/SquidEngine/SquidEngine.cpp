

#include "SquidEngine.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "SampleShapes.h"


using namespace std;

void mainResizeEvent(GLFWwindow* window, int width, int height)
{
	cout << "Resized to :" << width << ", " << height << endl;
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
	ViewPort view1(0.0f, 0.5f,0.5f,1.0f);
	ViewPort view2(0.5f, 0.5f, 1.0f, 1.0f);

	ShaderProgram shader("VertexShader.vert", nullptr, "FragmentShader.frag");

	Square square(0,0,0);
	square.build();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);


	while (!mainWindow.closing())
	{
		processInput(mainWindow);

		viewMain.use();

		glClearColor(0.43f, 0.71f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		
		square.draw(shader);

		glfwSwapBuffers(glfwGetCurrentContext());
		glfwPollEvents();
	}	

	return 0;
}

