#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/opengl_objects/ShaderProgram.hpp"
#include "src/drawables/square.hpp"
#include <vector>
#include <iostream>

#define WIDTH 1000
#define HEIGHT 1000

void windowResizeCallback(GLFWwindow* window, int width, int height)
{

}

int main(void)
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);
    
    Engine::ShaderProgram shad1("src/shaders/default.vert", "src/shaders/default.frag");
    Engine::Square square1(0, 0, 0.2, shad1.getId());



    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.1, 0.0);

        square1.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}