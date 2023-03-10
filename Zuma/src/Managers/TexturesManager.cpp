#include "TexturesManager.hpp"

bool TexturesManager::createTexture(std::string id, std::string texturePath)
{
	std::map <std::string, Texture*>::iterator it = textures.find(id);
	if (it == textures.end())
	{
		textures.insert({ id, new Texture(texturePath) });
		return true;
	}
	else
	{
		return false;
	}
}

bool TexturesManager::deleteTexture(std::string id)
{
	std::map <std::string, Texture*>::iterator it = textures.find(id);
	if (it != textures.end())
	{
		delete (*it).second;
		textures.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

Texture* TexturesManager::getTexture(std::string id)
{
	std::map <std::string, Texture*>::iterator it = textures.find(id);
	if (it != textures.end())
	{
		return (*it).second;
	}
	else
	{
		return nullptr;
	}
}

std::map<std::string, Texture*>::iterator TexturesManager::begin()
{
	return textures.begin();
}

std::map<std::string, Texture*>::iterator TexturesManager::end()
{
	return textures.end();
}