#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/OpenGL/ShaderProgram.hpp"
#include "src/OpenGL/FrameBuffer.hpp"
#include "src/zumalogic/CubicBezierSpline.hpp"
#include "src/Graphics/SpriteRenderer.hpp"
#include "src/zumalogic/gameobject.hpp"
#include "src/zumalogic/GameLevel.hpp"
#include <vector>
#include <iostream>
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
#include "src/Managers/TexturesManager.hpp"

#define WIDTH 1920
#define HEIGHT 1080

int main(void)
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Zuma", NULL, NULL);

    glfwSetWindowSize(window, WIDTH, HEIGHT);

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, WIDTH, HEIGHT);

    ShaderProgram spriteShad("src/shaders/sprite.vert", "src/shaders/sprite.frag");
    ShaderProgram screenShad("src/shaders/screen.vert", "src/shaders/screen.frag");
    SpriteRenderer renderer(spriteShad.getId(), window);
    TexturesManager texturesManager;

    texturesManager.createTexture("red", "src/textures/red.jpg");
    texturesManager.createTexture("path", "src/textures/path4.jpg");
    texturesManager.createTexture("frog", "src/textures/frog.png");
    texturesManager.createTexture("control_point1", "src/textures/control_point.png");
    texturesManager.createTexture("control_point2", "src/textures/control_point2.png");
    texturesManager.createTexture("marble1", "src/textures/marble1.png");
    texturesManager.createTexture("marble2", "src/textures/marble2.png");
    texturesManager.createTexture("marble3", "src/textures/marble3.png");

    Texture red("src/textures/red.jpg");
    Texture path("src/textures/path4.jpg");
    Texture frog("src/textures/frog.png");
    Texture control_point1("src/textures/control_point.png");
    Texture control_point2("src/textures/control_point2.png");
    Texture marble1("src/textures/marble1.png");
    Texture marble2("src/textures/marble2.png");
    Texture marble3("src/textures/marble3.png");

    std::vector<Texture*> allTextures;
    allTextures.push_back(texturesManager.getTexture("red"));
    allTextures.push_back(texturesManager.getTexture("path"));
    allTextures.push_back(texturesManager.getTexture("frog"));
    allTextures.push_back(texturesManager.getTexture("control_point1"));
    allTextures.push_back(texturesManager.getTexture("control_point2"));
    allTextures.push_back(texturesManager.getTexture("marble1"));
    allTextures.push_back(texturesManager.getTexture("marble2"));
    allTextures.push_back(texturesManager.getTexture("marble3"));

    std::vector<Texture*> segmentTextures;
    segmentTextures.push_back(texturesManager.getTexture("marble1"));
    segmentTextures.push_back(texturesManager.getTexture("marble2"));
    segmentTextures.push_back(texturesManager.getTexture("marble3"));

    std::vector<int> init = { 0,0,0,1,0,0,0,2,2,1,0,0 };

    CubicBezierSpline* route = new CubicBezierSpline(100, 100, 20, texturesManager.getTexture("control_point1"), texturesManager.getTexture("control_point2"), &red, 15);
    Snake* initialSnake = new Snake(20, 2, route, 200, &segmentTextures);
    Shooter* shooter = new Shooter(1000, 500, 100, texturesManager.getTexture("frog"), false, &segmentTextures, initialSnake->getSegmentSize(), 10);
    SnakeManager* manager = new SnakeManager(shooter, initialSnake);
    GameLevel level1(route, shooter, nullptr, nullptr, manager);

    setupCallbacks(window);

    std::vector<float> screenVertices = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    VertexArray screenVAO;
    screenVAO.bind();
    VertexBuffer screenVBO(screenVertices);
    screenVBO.bind();
    screenVAO.setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    screenVAO.setAttributePointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, sizeof(float) * 2);
    screenVAO.unbind();

    FrameBuffer screenBuffer(WIDTH, HEIGHT);

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        //screenBuffer.bindFrameBuffer();
        glClearColor(0.15, 0.15, 0.15, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        level1.draw(&renderer);

        /*screenBuffer.unbindFrameBuffer();
        glClearColor(0.15, 0.15, 0.15, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        screenShad.bind();
        std::cout << "TEX:" << screenBuffer.getTextureID() << std::endl;
        glBindTexture(GL_TEXTURE_2D, screenBuffer.getTextureID());
        screenVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        screenVAO.unbind();*/

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

        ImGui::SetNextWindowPos(ImVec2(1500, 0));
        ImGui::Begin("Snakes");
        ImGui::TextColored(ImVec4(1, 0.5, 1, 1), "Sneks");
        ImGui::BeginChild("Snaks");

        std::list<Snake*>::iterator it = manager->getSnakes()->begin();

        int n = 0;
        while (it != level1.manager->getSnakes()->end())
        {
            ImGui::Text("%d | %d", n, (*it)->getSegments()->size());
            it++;
            n++;
        }

        ImGui::NextColumn();
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
        if (ImGui::TreeNode("Texture Loader"))
        {
            textureLoader(&texturesManager, &segmentTextures, &level1);
            ImGui::TreePop();
            ImGui::Separator();
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

        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        std::cout << duration.count() << std::endl;
        if (duration.count() < 16)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(16 - duration.count()));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(0));
        }
    }

    glfwTerminate();
    return 0;
}