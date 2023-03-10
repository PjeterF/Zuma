#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CubicBezierSpline.hpp"
#include "Shooter.hpp"

enum InputModes { NOTHING, ADD_REMOVE_SEGMENT, MOVE_CTRL_POINTS, SHOOT };
static int inputState = InputModes::NOTHING;
static int splineIndex = -1;
static bool movingCtrlPoint = false;

void cursor_move_callback(GLFWwindow* window, double xpos, double ypos);
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void setupCallbacks(GLFWwindow* window)
{
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, cursor_move_callback);
    glfwSetKeyCallback(window, key_callback);
}

void cursor_move_callback(GLFWwindow* window, double xpos, double ypos)
{
   //std::cout << xpos << ", " << ypos << "\n";
    if (movingCtrlPoint)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        CubicBezierSpline* spline = (CubicBezierSpline*)glfwGetWindowUserPointer(window);
        spline->getControlPoints()->at(splineIndex) = glm::vec2(xpos, height-ypos);
        spline->sampleSpline();
    }
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (inputState)
    {
    case InputModes::ADD_REMOVE_SEGMENT:
    {
        if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        {
            CubicBezierSpline* spline = (CubicBezierSpline*)glfwGetWindowUserPointer(window);
            spline->removeLastSegment();
        }
        break;
    }
    default:
        break;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
    ImGuiIO& io = ImGui::GetIO();

    // (2) ONLY forward mouse data to your underlying app/game.
    if (!io.WantCaptureMouse)
    {
        double x, y;
        int width, height;
        glfwGetCursorPos(window, &x, &y);
        glfwGetWindowSize(window, &width, &height);
        y = height - y;
        //glm::vec2 conv = glm::vec2(-1 + 2 * (x / (width)), 1 - 2 * (y / height));
        switch (inputState)
        {
        case InputModes::ADD_REMOVE_SEGMENT:
        {
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            {
                CubicBezierSpline* spline = (CubicBezierSpline*)glfwGetWindowUserPointer(window);
                spline->addSegment(x, y);
            }
            break;
        }
        case InputModes::MOVE_CTRL_POINTS:
        {
            CubicBezierSpline* spline = (CubicBezierSpline*)glfwGetWindowUserPointer(window);
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            {
                float delta = 20;
                float distance;
                std::vector < glm::vec2>* points = spline->getControlPoints();
                for (int i = 0; i < points->size(); i++)
                {
                    if (x > points->at(i).x - delta && x < points->at(i).x + delta &&
                        y > points->at(i).y - delta && y < points->at(i).y + delta)
                    {
                        movingCtrlPoint = true;
                        splineIndex = i;
                        break;
                    }
                }
            }

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && splineIndex != -1)
            {
                movingCtrlPoint = false;
                spline->getControlPoints()->at(splineIndex) = glm::vec2(x, y);
                spline->sampleSpline();
                splineIndex = -1;
            }

            break;
        }
        case InputModes::SHOOT:
        {
            Shooter* shooter = (Shooter*)glfwGetWindowUserPointer(window);
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            {
                shooter->shoot(x, y, 0);
            }
            break;
        }
        default:
            break;
        }
    }
}