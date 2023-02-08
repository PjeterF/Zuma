#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(int width, int height)
{
	glGenBuffers(1, &id_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, id_buffer);

	glGenTextures(1, &id_texture);
	glBindTexture(GL_TEXTURE_2D, id_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_TEXTURE_2D, id_texture, 0);

	glGenBuffers(1, &id_render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, id_render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id_render_buffer);
}