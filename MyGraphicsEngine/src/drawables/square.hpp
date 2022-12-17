#include "../opengl_objects/ElementBuffer.hpp"
#include "../opengl_objects/VertexArray.hpp"
#include "../opengl_objects/VertexBuffer.hpp"
#include <vector>

namespace Engine
{
	class Square
	{
	public:
		Square(float x, float y, float size, GLuint shaderProgramID);
		void draw();
		void changePos(float x, float y);
	private:
		Engine::VertexBuffer* VBO;
		Engine::VertexArray* VAO;
		Engine::ElementBuffer* EBO;
		GLuint shaderProgramID;
		float x;
		float y;
		float size;
	};
}