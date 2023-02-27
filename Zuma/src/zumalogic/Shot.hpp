#pragma once

#include "GameObject.hpp"

class Shot : public GameObject
{
public:
	Shot(float x, float y, float size, Texture* texture,  float velocity, glm::vec2 direction);
	float getVelocity() { return velocity; }
	glm::vec2 getDirection() { return direction; }
	void setVelocity(float velocity) { this->velocity=velocity; }
	void setDirection(glm::vec2 direction) { this->direction = glm::normalize(direction); }
private:
	float velocity;
	glm::vec2 direction;
};