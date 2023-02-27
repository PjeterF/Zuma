#pragma once
#include "Shot.hpp"

Shot::Shot(float x, float y, float size, Texture* texture, float velocity, glm::vec2 direction) : GameObject(x, y, size, texture, GameObject::generateID(), true)
{
	this->velocity = velocity;
	this->direction = glm::normalize(direction);
}