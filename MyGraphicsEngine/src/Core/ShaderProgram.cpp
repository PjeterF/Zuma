#include "ShaderProgram.hpp"

namespace Engine
{
	ShaderProgram::ShaderProgram(std::string vertexSourceFilepath, std::string fragmentSourceFilepath)
	{
		const char* vertexSource = readFile(vertexSourceFilepath);
		const char* fragmentSource = readFile(fragmentSourceFilepath);

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);

		id = glCreateProgram();

		glAttachShader(id, vertexShader);
		glAttachShader(id, fragmentShader);
		glLinkProgram(id);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void ShaderProgram::bind()
	{
		glUseProgram(id);
	}

	void ShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	unsigned int ShaderProgram::getId()
	{
		return id;
	}

	const char* ShaderProgram::readFile(std::string filepath)
	{
		std::string shaderSourceCode;
		std::string line = "";
		std::ifstream file(filepath);

		if (!file)
		{
			return nullptr;
		}
		else
		{
			while (!file.eof())
			{
				std::getline(file, line);
				shaderSourceCode.append(line + '\n');
			}

			file.close();

			char* retu = new char[shaderSourceCode.length() + 1];
			strcpy_s(retu, shaderSourceCode.length() + 1, shaderSourceCode.c_str());

			return retu;
		}
	}
}