

#include "SquidEngine.h"
#include "Window.h"
#include "ShaderProgram.h"


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

#include "Shape.h"

class Square : public Shape {
public:
	Square(float x, float y, float z) : Shape(x, y, z) {}

	void build() {

		vertices = {
			 Vertex(0.5f,  0.5f, 0.0f, 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f),  // top right
			 Vertex(0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f),  // bottom right
			 Vertex(-0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f),  // bottom left
			 Vertex(-0.5f,  0.5f, 0.0f, 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f)   // top left 
		};

		indices = {
			0, 1, 3,
			1, 2, 3
		};

		createBuffer();
	}
};

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

