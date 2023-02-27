#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/vec2.hpp"
#include "CubicBezierSpline.hpp"

#define WIDTH 1000
#define HEIGHT 1000

void addSegment(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        glfwGetCursorPos(window, &x, &y);
        glm::vec2 converted = glm::vec2(-1 + 2 * (x / (WIDTH)), 1 - 2 * (y / HEIGHT));
        CubicBezierSpline* spline = (CubicBezierSpline*)glfwGetWindowUserPointer(window);
        spline->addSegment(converted.x, converted.y);
    }
}

void addSegment_button(GLFWwindow* window, CubicBezierSpline* spline)
{
    static bool addSegment_enabled = false;
    if (ImGui::Button("Add segment"))
    {
        std::cout << "Clicked";
        if (addSegment_enabled)
        {
            addSegment_enabled = false;
            glfwSetWindowUserPointer(window, nullptr);
            glfwSetMouseButtonCallback(window, nullptr);
            ImGui_ImplGlfw_InstallCallbacks(window);
        }
        else
        {
            addSegment_enabled = true;
            glfwSetWindowUserPointer(window, spline);
            glfwSetMouseButtonCallback(window, addSegment);
            ImGui_ImplGlfw_InstallCallbacks(window);
        }
    }
}
    


void spline_edit_menu(GLFWwindow* window, CubicBezierSpline* spline)
{

    ImGui::Begin("Spline edit");

    addSegment_button(window, spline);

    if (ImGui::Button("Move control point"))
    {

    }

    ImGui::End();
}