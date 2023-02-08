#pragma once

#include "GameObject.hpp"

class SnakeSegment : public GameObject
{
public:
	using GameObject::GameObject;
	int getSampleIndex() { return sampleIndex; }
	void setSampleIndex(int index) { sampleIndex= index; }
private:
	int sampleIndex=0;
};