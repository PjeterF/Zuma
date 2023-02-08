#include "application.hpp"

Application::Application(int width, int height, std::string windowName)
{
	this->width = width;
	this->height = height;
	this->windowName = windowName;

	initialize();
	initializeGLAD();
	initializeImGui();
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
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

void Application::initializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 300 es");
}

void Application::run()
{
	Engine::ShaderProgram spriteShaderProgram("src/shaders/sprite.vert", "src/shaders/sprite.frag");
	Engine::ShaderProgram defaultShad("src/shaders/default.vert", "src/shaders/default.frag");
	Engine::ShaderProgram texShad1("src/shaders/texturedQuad.vert", "src/shaders/texturedQuad.frag");

	Texture amogus("src/textures/amogus.jpg");
	Texture red("src/textures/red.jpg");

	std::vector<glm::vec2> ctrlpts;
	ctrlpts.push_back(glm::vec2(-0.9, 0.5));
	ctrlpts.push_back(glm::vec2(-0.8, 0));
	ctrlpts.push_back(glm::vec2(-0.7, 0));
	ctrlpts.push_back(glm::vec2(-0.6, 0));
	ctrlpts.push_back(glm::vec2(-0.5, 0.3));
	ctrlpts.push_back(glm::vec2(-0.4, 0));
	ctrlpts.push_back(glm::vec2(-0.3, 0.5));

	SpriteRenderer renderer(spriteShaderProgram.getId(), window);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0, 0.1, 0.2, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		renderer.draw(glm::vec2(0, 0), glm::vec2(0.5, 0.5), 45, &amogus);

		ImGui::Begin("Sprite 1");
		if (ImGui::Button("Button"))
		{
			
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}