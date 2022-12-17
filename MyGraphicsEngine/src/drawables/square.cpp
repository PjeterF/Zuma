#include "square.hpp"

namespace Engine
{
	Square::Square(float x, float y, float size, GLuint shaderProgramID)
	{
		this->x = x;
		this->y = y;
		this->size = size;
		this->shaderProgramID = shaderProgramID;

		std::vector<float> vertices;
		vertices.push_back(x-size);
		vertices.push_back(y+size);
		vertices.push_back(x+size);
		vertices.push_back(y+size);
		vertices.push_back(x+size);
		vertices.push_back(y-size);
		vertices.push_back(x-size);
		vertices.push_back(x-size);

		std::vector<int> indices;
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);

		VAO = new Engine::VertexArray;
		VAO->bind();

		VBO = new Engine::VertexBuffer(vertices);
		VBO->bind();

		EBO = new Engine::ElementBuffer(indices);
		EBO->bind();

		VAO->setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		VAO->unbind();
	}

	void Square::draw()
	{
		glUseProgram(shaderProgramID);
		VAO->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		VAO->unbind();
	}
}