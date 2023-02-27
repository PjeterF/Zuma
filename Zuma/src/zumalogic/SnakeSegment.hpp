#pragma once

#include "GameObject.hpp"
#include"CubicBezierSpline.hpp"

class SnakeSegment : public GameObject
{
public:
	SnakeSegment(Texture* texture, float size);
	SnakeSegment(float x, float y, float size, Texture* texture);
	int getSampleIndex() { return sampleIndex; }
	void setSampleIndex(int index) { sampleIndex= index; }
	bool hasSpawned() { return spawned; }
	void spawn(CubicBezierSpline* route);
private:
	int sampleIndex=0;
	bool spawned;
};