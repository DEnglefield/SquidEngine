

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

	while (!mainWindow.closing())
	{
		processInput(mainWindow);
		int w, h;
		mainWindow.getWindowSize(&w,&h);

		glViewport(0, h/2, w/2, h);
		glScissor(0, h/2, w/2, h);

		glClearColor(0.43f, 0.71f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glViewport(w/2, h/2, w, h);
		glScissor(w/2, h/2, w, h);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mainWindow.swapBuffers();
		//glfwSwapBuffers(mainWindow.form);
		glfwPollEvents();
	}

	return 0;
}

