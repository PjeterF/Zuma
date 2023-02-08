#include "GameLevel.hpp"

GameLevel::GameLevel(BezierCubicSpline* route, Shooter* shooter, Texture* background, std::vector <GameObject*>* ornaments, SnakeManager* manager)
{
	this->route = route;
	this->shooter = shooter;
	this->background = background;
	this->manager = manager;

	if (ornaments != nullptr)
	{
		for (auto object : *ornaments)
		{
			this->ornaments->push_back(object);
		}
	}
}

void GameLevel::update(GLFWwindow* window, float angle)
{
	manager->update(window);
	shooter->setAngle(angle);
}

void GameLevel::draw(SpriteRenderer* renderer)
{
	int width, height;
	glfwGetWindowSize(renderer->getWindowPointer(), &width, &height);

	int n, m;
	n = width / background->getWidth();
	m = height / background->getHeight();



	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			renderer->draw(glm::vec2(i*2*background->getWidth(), j *2* background->getHeight()), glm::vec2(background->getWidth(), background->getHeight()), 0, this->background);
		}
	}

	this->route->draw(renderer, 5);
	this->route->drawControlPoints(renderer);

	if(shooter!=nullptr)
		this->shooter->draw(renderer);

	if(ornaments!=nullptr)
	{
		for (auto ornament : *ornaments)
		{
			ornament->draw(renderer);
		}
	}

	manager->draw(renderer);
}