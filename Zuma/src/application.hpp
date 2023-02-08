#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#include "drawables/spriterenderer.hpp"
#include "zumalogic/beziercubicspline.hpp"
#include "opengl_objects/texture.hpp"
#include "opengl_objects/ShaderProgram.hpp"

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
	void initializeImGui();
	int width;
	int height;
	std::string windowName;
	GLFWwindow* window;
};