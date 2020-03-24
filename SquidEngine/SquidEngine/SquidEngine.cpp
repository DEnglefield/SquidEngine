

#include "SquidEngine.h"
#include "Window.h"

using namespace std;

void mainResizeEvent(GLFWwindow* window, int width, int height)
{
	//glViewport(0, 0, width, height);
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

	glEnable(GL_SCISSOR_TEST);


	ViewPort view1(0.0f, 0.5f,0.5f,1.0f);
	ViewPort view2(0.5f, 0.5f, 1.0f, 1.0f);

	while (!mainWindow.closing())
	{
		processInput(mainWindow);
		
		view1.use();

		glClearColor(0.43f, 0.71f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		view2.use();

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mainWindow.swapBuffers();
		//glfwSwapBuffers(mainWindow.form);
		glfwPollEvents();
	}

	return 0;
}

