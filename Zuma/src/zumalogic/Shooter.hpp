#pragma once

#include "GameObject.hpp"
#include "beziercubicspline.hpp"
#include "glm/vec2.hpp"
#include <vector>
#include <math.h>
#include "Snake.hpp"
#include "Shot.hpp"
#include <time.h>

class Shooter : public GameObject
{
public:
	Shooter(float x, float y, float size, Texture* texture, bool colission, std::vector<Texture*>* projectileTextures, float projectileSize, float projectileVelocity);
	void shoot(float xTarget, float yTarget, int projectileType);
	void updateProjectiles(GLFWwindow* window);
	void drawBody(SpriteRenderer* renderer);
	void drawProjectiles(SpriteRenderer* renderer);
	std::vector<Shot*>* getProjectiles() { return &projectiles; }
	void setProjectileSize(float newSize) { projectileSize = newSize; };
	void reroll();
	void setCurrentOffset(float newOffset);
	void setNextOffset(glm::vec2 newOffset);
private:
	int currentProjectileTag;
	int nextProjectileTag;
	std::vector<Shot*> projectiles;
	std::vector<Texture*>* projectileTextures;
	float projectileSize;
	float projectileVelocity;
	glm::vec2 currentOffset;
	glm::vec2 nextOffset;
};