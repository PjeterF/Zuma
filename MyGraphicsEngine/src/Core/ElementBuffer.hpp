#include <glad/glad.h>
#include <vector>

namespace Engine
{
	class ElementBuffer
	{
	public:
		ElementBuffer(std::vector<int>& indices);
		void bind();
		void unbind();
	private:
		unsigned int id;
	};
}