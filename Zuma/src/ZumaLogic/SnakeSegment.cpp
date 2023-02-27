#include "SnakeSegment.hpp"

SnakeSegment::SnakeSegment(Texture* texture, float size) : GameObject(0, 0, size, texture, GameObject::generateID(), false)
{
	spawned = false;
}

SnakeSegment::SnakeSegment(float x, float y, float size, Texture* texture) : GameObject(x, y, size, texture, GameObject::generateID(), true)
{
	spawned = true;
}

void SnakeSegment::spawn(CubicBezierSpline* route)
{
	if (route->getControlPoints()->size() > 0)
	{
		glm::vec2 firstPoint = route->getControlPoints()->at(0);
		position = glm::vec2(firstPoint.x, firstPoint.y);
		collision = true;
		spawned = true;
	}
}