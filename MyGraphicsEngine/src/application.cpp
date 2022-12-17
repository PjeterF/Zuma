#include "application.hpp"

Application::Application(int width, int height, std::string windowName)
{
	this->width = width;
	this->height = height;
	this->windowName = windowName;

	initialize();
	initializeGLAD();
}

Application::~Application()
{
	glfwTerminate();
}

bool Application::initialize()
{
	glfwInit();
	window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);

	if (window == NULL)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	return true;
}

bool Application::initializeGLAD()
{
	if (!gladLoadGL())
	{
		return false;
	}
	glViewport(0, 0, width, height);
	return true;
}

void Application::run()
{

}