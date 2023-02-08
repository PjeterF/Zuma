#pragma once
#include <gladES/glad.h>
class FrameBuffer
{
public:
	FrameBuffer(int width, int height);  
private:
	unsigned int id_buffer;
	unsigned int id_texture;
	unsigned int id_render_buffer;
};

