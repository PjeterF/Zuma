#pragma once
#include "../opengl_objects/texture.hpp"
#include "../drawables/spriterenderer.hpp"
#include "glm/vec2.hpp"
#include <math.h>

class GameObject
{
public:
	GameObject(float x, float y, float size, Texture* texture, int ID, bool collision);
	void draw(SpriteRenderer* spriteRenderer);
	void setPosition(float x, float y);
	void setScale(float scale);
	glm::vec2 getPosition();
	glm::vec2 getScale() { return scale; }
	void setTexture(Texture* texture);
	bool checkCollision(GameObject* other);
	int getID();
	int getTag() { return tag; }
	int getAngle() { return angle; }
	void setTag(int tag) { this->tag=tag; }
	void setAngle(float angle) { this->angle = angle; };
	static int generateID() { return ID_GEN++; };
protected:
	int ID;
	static int ID_GEN;
	bool collision;
	glm::vec2 position;
	glm::vec2 scale;
	float angle = 0;
	Texture* texture;
	int tag = -1;

};