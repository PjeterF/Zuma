#pragma once

#include "Snake.hpp"
#include "Shooter.hpp"
#include <list>
#include <iterator>
#include "glm/vec2.hpp"

class SnakeManager
{
public:
	SnakeManager(Shooter* shooter, Snake* initialSnake);
	void push_back_snake(Snake* snake);
	void push_front_snake(Snake* snake);
	void update(GLFWwindow* window);
	void draw(SpriteRenderer* renderer);
	std::list<Snake*>* getSnakes() { return &snakes; }
	Shooter* getShooter() { return shooter; }
	std::list<Snake*>::iterator snakesBeginIterator() { return snakes.begin(); }
	std::list<Snake*>::iterator snakesEndIterator() { return snakes.end(); }
	bool replaceShooter(Shooter* shooter);
	bool replaceInitialSnake(Snake* snake);
private:
	void updatePos();
	void checkForHits();
	bool checkPopingCondition(std::list<SnakeSegment*>::iterator it, std::list<Snake*>::iterator it_snake, int count);
	void popSame(std::list<SnakeSegment*>::iterator it, std::list<Snake*>::iterator it_snake);
	std::list<Snake*> snakes;
	Shooter* shooter;
	int popingCount = 3;
};