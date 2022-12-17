#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#define WIDTH 1000
#define HEIGHT 1000

class Application
{
public:
	Application(int width, int height, std::string windowName);
	~Application();
	void run();
protected:
	bool initialize();
	bool initializeGLAD();
	int width;
	int height;
	std::string windowName;
	GLFWwindow* window;
};