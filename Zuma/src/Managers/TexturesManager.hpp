#pragma once
#include <map>
#include <string>

#include "../OpenGL/Texture.hpp"
class TexturesManager
{
public:
	TexturesManager() {};
	bool createTexture(std::string id, std::string texturePath);
	bool deleteTexture(std::string id);
	Texture* getTexture(std::string id);
private:
	std::map<std::string, Texture*> textures;
};

