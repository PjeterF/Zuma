#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/opengl_objects/ShaderProgram.hpp"
#include "src/opengl_objects/FrameBuffer.hpp"
#include "src/zumalogic/beziercubicspline.hpp"
#include "src/drawables/SpriteRenderer.hpp"
#include "src/zumalogic/gameobject.hpp"
#include "src/zumalogic/GameLevel.hpp"
#include <vector>
#include <iostream>
#include "src/application.hpp"
#include "src/zumalogic/GuiElements.hpp"
#include "src/zumalogic/Snake.hpp"
#include <chrono>
#include <thread>
#include "src/zumalogic/Input.hpp"
#include <ctime>
#include <math.h>
#include "src/zumalogic/Shooter.hpp"
#include "src/zumalogic/SnakeManager.hpp"
#include "src//imguiWidgets.hpp"

#define WIDTH 1920
#define HEIGHT 1080


int main(void)
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);

    glfwSetWindowSize(window, WIDTH, HEIGHT);

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, WIDTH, HEIGHT);

    Engine::ShaderProgram spriteShad("src/shaders/sprite.vert", "src/shaders/sprite.frag");
    SpriteRenderer renderer(spriteShad.getId(), window);

    Texture red("src/textures/red.jpg");
    Texture path("src/textures/path4.jpg");
    Texture frog("src/textures/frog.png");
    Texture control_point1("src/textures/control_point.png");
    Texture control_point2("src/textures/control_point2.png");
    Texture marble1("src/textures/marble1.png");
    Texture marble2("src/textures/marble2.png");
    Texture marble3("src/textures/marble3.png");

    std::vector<Texture*> allTextures;
    allTextures.push_back(&red);
    allTextures.push_back(&path);
    allTextures.push_back(&frog);
    allTextures.push_back(&control_point1);
    allTextures.push_back(&control_point2);
    allTextures.push_back(&marble1);
    allTextures.push_back(&marble2);
    allTextures.push_back(&marble3);

    std::vector<Texture*> segmentTextures;
    segmentTextures.push_back(&marble1);
    segmentTextures.push_back(&marble2);
    segmentTextures.push_back(&marble3);

    std::vector<int> init = { 0,0,0,1,0,0,0,2,2,1,0,0 };

    BezierCubicSpline* route = new BezierCubicSpline(100, 100, 20, &control_point1, &control_point2, &red, 15);
    Snake* initialSnake = new Snake(20, 2, route, 200, &segmentTextures);
    Shooter* shooter = new Shooter(1000, 500, 100, &frog, &segmentTextures, initialSnake->getSegmentSize(), 10);
    SnakeManager* manager = new SnakeManager(shooter, initialSnake);
    GameLevel level1(route, shooter, &path, nullptr, manager);

    setupCallbacks(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 300 es"); 

    bool running = false;
    bool shootingEnabled = false;
    bool readyToShoot = false;
    time_t begin, end;
    int deltaT;
    double x, y;
    float angle = 0;
    int segmentType = 0;
    double xpos, ypos;
    glm::vec2 vector;

    FrameBuffer frameBuffer(WIDTH, HEIGHT);

    std::vector<float> ScreenRect = {
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
    };

    std::vector<int> ScreenIndices =
    {
        0, 1, 2,
        1, 2, 3
    };

    Engine::VertexArray ScreenVAO;
    ScreenVAO.bind();
    Engine::VertexBuffer ScreenVBO(ScreenRect);
    ScreenVBO.bind();
    Engine::ElementBuffer ScreenEBO(ScreenIndices);
    ScreenEBO.bind();

    ScreenVAO.setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    ScreenVAO.setAttributePointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, sizeof(float) * 2);

    Engine::ShaderProgram ScreenProgram("src/shaders/screen.vert", "src/shaders/screen.frag");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        /*frameBuffer.bind();
        glEnable(GL_DEPTH_TEST);*/

        glClearColor(0.2, 0.2, 0.2, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        level1.draw(&renderer);
        /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ScreenProgram.bind();
        ScreenVAO.bind();
        glBindTexture(GL_TEXTURE_2D, frameBuffer.getTextureID());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

        if (running)
        {
            level1.update(window, angle);
        }

        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = HEIGHT - ypos;
        vector = glm::vec2(xpos, ypos) - level1.shooter->getPosition();
        vector = glm::normalize(vector);
        if (xpos >= level1.shooter->getPosition().x && ypos >= level1.shooter->getPosition().y)
        {
            angle = atan(vector.y / vector.x) * 57.2958 + 90;
        }
        else if (xpos < level1.shooter->getPosition().x && ypos >= level1.shooter->getPosition().y)
        {
            angle = 180 - atan(-vector.y / vector.x) * 57.2958 + 90;
        }
        else if (xpos < level1.shooter->getPosition().x && ypos < level1.shooter->getPosition().y)
        {
            angle = 180 + atan(vector.y / vector.x) * 57.2958 + 90;
        }
        else if (xpos >= level1.shooter->getPosition().x && ypos < level1.shooter->getPosition().y)
        {
            angle = 360 - atan(-vector.y / vector.x) * 57.2958 + 90;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(500, 0));
        ImGui::Begin("Objects");
        ImGui::TextColored(ImVec4(1, 0.5, 1, 1), "Shots");
        ImGui::BeginChild("List");

        ImGui::Columns(3);
        ImGui::Text("ID");
        ImGui::NextColumn();
        ImGui::Text("X");
        ImGui::NextColumn();
        ImGui::Text("Y");
        ImGui::NextColumn();

        ImGui::Columns(3);
        for (int i = 0; i < level1.shooter->getProjectiles()->size(); i++)
        {
            Shot* currentShot = level1.shooter->getProjectiles()->at(i);
            ImGui::Text("%d", currentShot->getID());
        }
        ImGui::NextColumn();
        for (int i = 0; i < level1.shooter->getProjectiles()->size(); i++)
        {
            Shot* currentShot = level1.shooter->getProjectiles()->at(i);
            ImGui::Text("%d", (int)currentShot->getPosition().x);
        }
        ImGui::NextColumn();
        for (int i = 0; i < level1.shooter->getProjectiles()->size(); i++)
        {
            Shot* currentShot = level1.shooter->getProjectiles()->at(i);
            ImGui::Text("%d", (int)currentShot->getPosition().y);
        }
        ImGui::EndChild();
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Menus", nullptr, ImGuiWindowFlags_NoMove);

        
        if (ImGui::Button("Pause/Unpause"))
        {
            if (running)
            {
                running = false;
            }
            else
            {
                running = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Enable/Disable Shooting"))
        {
            if (shootingEnabled)
            {
                shootingEnabled = false;
                inputState = InputModes::NOTHING;
            }
            else
            {
                inputState = InputModes::SHOOT;
                glfwSetWindowUserPointer(window, level1.shooter);
                shootingEnabled = true;
            }
        }
        if (ImGui::TreeNode("Route Edit"))
        {
            if (ImGui::Button("Add(LMB)/Remove(Z) segment"))
            {
                inputState = InputModes::ADD_REMOVE_SEGMENT;
                glfwSetWindowUserPointer(window, level1.route);
            }
            if (ImGui::Button("Move control points(LMB drag)"))
            {
                inputState = InputModes::MOVE_CTRL_POINTS;
                glfwSetWindowUserPointer(window, level1.route);
            }
            ImGui::TreePop();
            ImGui::Separator();
        }
        if (ImGui::TreeNode("Texture Loader"))
        {
            textureLoader(&allTextures, &segmentTextures, &level1);
            ImGui::TreePop();
            ImGui::Separator();
        }
        if (ImGui::TreeNode("Snake Edit"))
        {
            snakeEdit(level1.route, level1.manager, &segmentTextures);
            ImGui::TreePop();
            ImGui::Separator();
        }
        if (ImGui::TreeNode("Shooter Edit"))
        {
            shooterEdit(level1.manager, window);
            ImGui::TreePop();
            ImGui::Separator();
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    glfwTerminate();
    return 0;
}