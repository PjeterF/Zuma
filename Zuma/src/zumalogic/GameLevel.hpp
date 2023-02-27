#pragma once

#include "CubicBezierSpline.hpp"
#include "GameObject.hpp"
#include "Shooter.hpp"
#include "SnakeManager.hpp"
#include "SnakeManager.hpp"
#include <vector>

class GameLevel
{
public:
	GameLevel(CubicBezierSpline* route, Shooter* shooter, Texture* background, std::vector <GameObject*>* ornaments, SnakeManager* manager);
	void draw(SpriteRenderer* renderer);
	void update(GLFWwindow* window, float angle);
	void setBackground(Texture* newBackground);
	CubicBezierSpline* route;
	Shooter* shooter;
	Texture* background;
	SnakeManager* manager;
	std::vector < GameObject*>* ornaments;
};