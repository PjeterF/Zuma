#pragma once

#include "GameObject.hpp"
#include "beziercubicspline.hpp"
#include "glm/vec2.hpp"
#include <vector>
#include <math.h>
#include "Snake.hpp"
#include "Shot.hpp"
#include <time.h>

class Shooter
{
public:
	Shooter(float x, float y, float size, Texture* texture, std::vector<Texture*>* projectileTextures, float projectileSize, float projectileVelocity);
	void shoot(float xTarget, float yTarget, int projectileType);
	void updateProjectiles(GLFWwindow* window);
	void draw(SpriteRenderer* renderer);
	void drawProjectiles(SpriteRenderer* renderer);
	std::vector<Shot*>* getProjectiles() { return &projectiles; }
	void setAngle(float degrees) { shooter->setAngle(degrees); }
	glm::vec2 getPosition() { return shooter->getPosition(); }
	void setTexture(Texture* texture) { shooter->setTexture(texture); };
	void setProjectileSize(float newSize) { projectileSize = newSize; };
	void reroll();
	glm::vec2 getScale() { return shooter->getScale(); }
	void setScale(float newScale) { shooter->setScale(newScale); }
	void setCurrentOffset(float newOffset);
	void setNextOffset(glm::vec2 newOffset);
	void setPosition(glm::vec2 newPos) { shooter->setPosition(newPos.x, newPos.y); }
private:
	int currentProjectileTag;
	int nextProjectileTag;
	GameObject* shooter;
	std::vector<Shot*> projectiles;
	std::vector<Texture*>* projectileTextures;
	float projectileSize;
	float projectileVelocity;
	glm::vec2 currentOffset;
	glm::vec2 nextOffset;
};