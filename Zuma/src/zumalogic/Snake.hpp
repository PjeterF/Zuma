#pragma once

#include "GameObject.hpp"
#include "beziercubicspline.hpp"
#include "glm/vec2.hpp"
#include "SnakeSegment.hpp"

#include <iterator>
#include <list>
#include <math.h>
#include <time.h>

class Snake
{
public:
	Snake(float segmentSize, float velocity, BezierCubicSpline* route, std::vector<int>& initVector , std::vector<Texture*>* textures);
	Snake(float segmentSize, float velocity, BezierCubicSpline* route, int snakeLength, std::vector<Texture*>* textures);
	void draw(SpriteRenderer* renderer);
	void update();
	glm::vec2 getHeadPos();
	glm::vec2 getTailPos();
	void mergeWith(Snake* other);
	bool spawnSegment(int type);
	Snake* split(std::list<SnakeSegment*>::iterator iterator);
	std::list<SnakeSegment*>* getSegments() { return &segments; }
	Texture* getTexture(int index);
	float getSegmentSize() { return segmentSize; }
	std::list<SnakeSegment*>::iterator insert(float x, float y, int type, std::list<SnakeSegment*>::iterator it);
	static float getDeistanceBetweenSegments() { return distanceBetweenSegments; }
	static void setDeistanceBetweenSegments(float newDistance) { distanceBetweenSegments = newDistance; }
	BezierCubicSpline* getRoute() { return route; }
private:
	Snake() {};
	void initializeRandom(int n);
	static float distanceBetweenSegments;
	float segmentSize;
	int headSampleIndex;
	unsigned int length;
	float velocity;
	std::vector<Texture*>* textures;
	std::list<SnakeSegment*> segments;
	BezierCubicSpline* route;
	bool inserting = false;
};