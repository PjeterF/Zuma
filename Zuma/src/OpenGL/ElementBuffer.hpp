#include <gladES/glad.h>
#include <vector>

	class ElementBuffer
	{
	public:
		ElementBuffer(std::vector<int>& indices);
		void bind();
		void unbind();
	private:
		unsigned int id;
	};