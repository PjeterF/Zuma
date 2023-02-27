#include "SpriteRenderer.hpp"

SpriteRenderer::SpriteRenderer(GLuint shaderProgramID, GLFWwindow* window)
{
	this->shaderProgramID = shaderProgramID;
	this->window = window;

	std::vector<float> vertices;
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(0);
	vertices.push_back(1);

	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(1);

	vertices.push_back(-1);
	vertices.push_back(-1);
	vertices.push_back(0);
	vertices.push_back(0);

	vertices.push_back(1);
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(0);

	std::vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	VAO = new VertexArray;
	VAO->bind();

	VBO = new VertexBuffer(vertices);
	VBO->bind();

	EBO = new ElementBuffer(indices);
	EBO->bind();

	VAO->setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	VAO->setAttributePointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, sizeof(float) * 2);

	VAO->unbind();
}

void SpriteRenderer::draw(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture)
{
	glm::mat4 transform = glm::mat4(1.0f);

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if (height <= 0)
	{
		height = 1;
	}
	float aspectRatio = width / height;


	
	//transform = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
	transform = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
	//transform = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	transform = glm::translate(transform, glm::vec3(position.x, position.y, 0));
	transform = glm::rotate(transform, glm::radians(angle+180), glm::vec3(0, 0, 1));
	transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 1));

	glUseProgram(shaderProgramID);
	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
	texture->bind();
	VAO->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}