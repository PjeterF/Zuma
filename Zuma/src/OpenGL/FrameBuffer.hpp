#pragma once
#include <gladES/glad.h>
#include <iostream>
class FrameBuffer
{
public:
	FrameBuffer(int width, int height);  
	void bindFrameBuffer();
	void unbindFrameBuffer();
	unsigned int getTextureID() { return id_texture; }
	void bindTexture() { glBindTexture(GL_TEXTURE_2D, id_texture); }
private:
	unsigned int id_buffer;
	unsigned int id_texture;
	unsigned int id_render_buffer;
};

