#pragma once

#include "GameObject.hpp"

class Shot : public GameObject
{
public:
	using GameObject::GameObject;
	float getVelocity() { return velocity; }
	glm::vec2 getDirection() { return direction; }
	void setVelocity(float velocity) { this->velocity=velocity; }
	void setDirection(glm::vec2 direction) { this->direction = direction; }
private:
	float velocity=10;
	glm::vec2 direction;
};