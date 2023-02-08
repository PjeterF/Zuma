#pragma once

#include "beziercubicspline.hpp"
#include "GameObject.hpp"
#include "Shooter.hpp"
#include "SnakeManager.hpp"
#include "SnakeManager.hpp"
#include <vector>

class GameLevel
{
public:
	GameLevel(BezierCubicSpline* route, Shooter* shooter, Texture* background, std::vector <GameObject*>* ornaments, SnakeManager* manager);
	void draw(SpriteRenderer* renderer);
	void update(GLFWwindow* window, float angle);
	void setBackground(Texture* newBackground);
	BezierCubicSpline* route;
	Shooter* shooter;
	Texture* background;
	SnakeManager* manager;
	std::vector < GameObject*>* ornaments;
private:
	
};