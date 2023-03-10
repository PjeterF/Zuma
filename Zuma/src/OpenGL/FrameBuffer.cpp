#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(int width, int height)
{
	glGenBuffers(1, &id_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, id_buffer);

	glGenTextures(1, &id_texture);
	glBindTexture(GL_TEXTURE_2D, id_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id_texture, 0);
}

void FrameBuffer::bindFrameBuffer()
{
	glBindBuffer(GL_FRAMEBUFFER, id_buffer);
}

void FrameBuffer::unbindFrameBuffer()
{
	glBindBuffer(GL_FRAMEBUFFER, 0);
}