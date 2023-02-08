#include "Shooter.hpp"

Shooter::Shooter(float x, float y, float size, Texture* texture, std::vector<Texture*>* projectileTextures, float projectileSize, float projectileVelocity)
{
	shooter = new GameObject(x, y, size, texture, GameObject::generateID(), false);
	this->projectileTextures = projectileTextures;
	this->projectileSize = projectileSize;
	this->projectileVelocity = projectileVelocity;
	srand(time(NULL));
	this->currentProjectileTag = rand() % projectileTextures->size();
	this->nextProjectileTag = rand() % projectileTextures->size();

	glm::vec2 direction(cos(glm::radians((float)(shooter->getAngle()) - 90)), sin(glm::radians((float)(shooter->getAngle() - 90))));
	direction = glm::normalize(direction);
	currentOffset = direction*shooter->getScale();
	nextOffset = glm::vec2(0, 0);
}

void Shooter::draw(SpriteRenderer* renderer)
{
	shooter->draw(renderer);
	if (projectileTextures->size() == 0)
	{
		return;
	}
	this->setCurrentOffset(sqrt(currentOffset.x * currentOffset.x + currentOffset.y * currentOffset.y));

	renderer->draw(shooter->getPosition()+ currentOffset, glm::vec2(projectileSize, projectileSize), 0, projectileTextures->at(currentProjectileTag));
	renderer->draw(shooter->getPosition()+nextOffset, glm::vec2(projectileSize, projectileSize), 0, projectileTextures->at(nextProjectileTag));
}

void Shooter::shoot(float xTarget, float yTarget, int projectileType)
{
	if (projectileTextures->size() == 0)
	{
		return;
	}
	Shot* newShot= new Shot(shooter->getPosition().x+currentOffset.x, shooter->getPosition().y+ currentOffset.y, projectileSize, projectileTextures->at(currentProjectileTag), GameObject::generateID(), true);
	newShot->setDirection(glm::vec2(xTarget, yTarget) - shooter->getPosition());
	newShot->setDirection(glm::normalize(newShot->getDirection()));
	newShot->setVelocity(this->projectileVelocity);
	newShot->setTag(currentProjectileTag);
	currentProjectileTag = nextProjectileTag;
	srand(time(NULL));
	this->nextProjectileTag = rand() % projectileTextures->size();
	projectiles.push_back(newShot);
}

void Shooter::updateProjectiles(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	int n = 0;
	for (auto projectile : projectiles)
	{
		projectile->setPosition(
			projectile->getPosition().x + projectile->getDirection().x * projectile->getVelocity(),
			projectile->getPosition().y + projectile->getDirection().y * projectile->getVelocity());
		if (projectile->getPosition().x < 0 || projectile->getPosition().y < 0 || projectile->getPosition().x > width || projectile->getPosition().y > height)
		{
			delete projectile;
			projectiles.erase(projectiles.begin() + n);
			return;
		}
		n++;
	}
}

void Shooter::drawProjectiles(SpriteRenderer* renderer)
{
	int n = 0;
	for (auto projectile : projectiles)
	{
		renderer->draw(projectile->getPosition(), projectile->getScale(), 0, projectileTextures->at(projectile->getTag()));

	}
}

void Shooter::reroll()
{
	if (projectileTextures->size() == 0)
	{
		currentProjectileTag = 0;
		nextProjectileTag = 0;
		return;
	}
	srand(time(NULL));
	currentProjectileTag = rand() % projectileTextures->size();
	nextProjectileTag = rand() % projectileTextures->size();
}

void Shooter::setCurrentOffset(float displacement)
{
	glm::vec2 direction(cos(glm::radians((float)(shooter->getAngle()) - 90)), sin(glm::radians((float)(shooter->getAngle() - 90))));
	direction = glm::normalize(direction);
	currentOffset = displacement * direction;
}

void Shooter::setNextOffset(glm::vec2 newOffset)
{
	nextOffset = newOffset;
}