#pragma once
#include <gladES/glad.h>
#include <iostream>

class Texture
{
public:
	Texture(std::string texturePath);
	~Texture();
	void bind();
	void unbind();
	int getWidth() { return width; }
	int getHeight() { return height; }
	GLuint getId() { return texture; }
private:
	GLuint texture;
	int width;
	int height;
};