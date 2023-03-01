#include "GameObject.hpp"

GameObject::GameObject(float x, float y, float size, Texture* texture, int ID, bool collision)
{
	position = glm::vec2(x, y);
	scale = glm::vec2(size, size);
	this->texture = texture;
	this->ID = ID;
	this->collision = collision;
}

GameObject::GameObject(float x, float y, float size, float angle, Texture* texture, bool collision)
{
	position = glm::vec2(x, y);
	scale = glm::vec2(size, size);
	this->angle = angle;
	this->texture = texture;
	this->collision = collision;
	this->ID = GameObject::generateID();
}

void GameObject::draw(SpriteRenderer* spriteRenderer)
{
	spriteRenderer->draw(position, scale, angle, texture);
}

void GameObject::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void GameObject::setTexture(Texture* texture)
{
	this->texture = texture;
}

glm::vec2 GameObject::getPosition()
{
	return position;
}

bool GameObject::checkCollision(GameObject* other)
{
	if (this->collision=false || other->collision==false)
	{
		return false;
	}

	glm::vec2 delta = position - other->position;

	float x = abs(delta.x) - (this->scale.x + other->scale.x);
	float y = abs(delta.y) - (this->scale.y + other->scale.y);

	if (x < 0 && y < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameObject::setScale(float scale)
{
	this->scale = glm::vec2(scale, scale);
}

int GameObject::getID()
{
	return ID;
}

int GameObject::ID_GEN = 0;
