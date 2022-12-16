#include <glad/glad.h>
#include <string>
#include <fstream>

namespace Engine
{
	class ShaderProgram
	{
	public:
		ShaderProgram(std::string vertexSourceFilepath, std::string fragmentSourceFilepath);
		void bind();
		void unbind();
		unsigned int getId();
	private:
		const char* readFile(std::string filepath);
		unsigned int id;
	};
}