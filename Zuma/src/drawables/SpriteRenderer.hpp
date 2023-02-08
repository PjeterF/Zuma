#pragma once
#include "../opengl_objects/package.hpp"
#include "glm/vec2.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"

class SpriteRenderer
{
public:
	SpriteRenderer(GLuint shaderProgramID, GLFWwindow* window);
	void draw(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture);
	GLFWwindow* getWindowPointer() { return window; }
protected:
	Engine::VertexBuffer* VBO;
	Engine::VertexArray* VAO;
	Engine::ElementBuffer* EBO;
	GLuint shaderProgramID;
	GLFWwindow* window;
};