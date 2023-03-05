#include "Snake.hpp"

Snake::Snake(float segmentSize, float velocity, CubicBezierSpline* route, std::vector<int>& initVector, std::vector<Texture*>* textures)
{
	this->segmentSize = segmentSize;
	this->route = route;
	this->length = 0;
	this->headSampleIndex = 0;
	this->velocity = velocity;
	this->textures = textures;

	float x, y;
	SnakeSegment* newSegment;
	for (int i = 0; i < initVector.size(); i++)
	{
		x = route->getControlPoints()->at(0).x;
		y = route->getControlPoints()->at(0).y;
		newSegment = new SnakeSegment(textures->at(initVector[i]), segmentSize);
		newSegment->setTag(initVector[i]);
		segments.push_back(newSegment);
	}

	last_unspawned = segments.begin();
}

Snake::Snake(float segmentSize, float velocity, CubicBezierSpline* route, int snakeLength, std::vector<Texture*>* textures)
{
	this->segmentSize = segmentSize;
	this->route = route;
	this->length = 0;
	this->headSampleIndex = 0;
	this->velocity = velocity;
	this->textures = textures;

	initializeRandom(snakeLength);

	last_unspawned = segments.begin();
}

void Snake::initializeRandom(int n)
{
	if (textures->size() == 0)
	{
		return;
	}
	float x, y;
	SnakeSegment* newSegment;
	srand(time(NULL));
	int random;
	for (int i = 0; i < n; i++)
	{
		x = route->getControlPoints()->at(0).x;
		y = route->getControlPoints()->at(0).y;
		random = rand() % textures->size();
		newSegment = new SnakeSegment(textures->at(random), segmentSize);
		newSegment->setTag(random);
		segments.push_back(newSegment);
	}
}

void Snake::draw(SpriteRenderer* renderer)
{
	for (auto element : segments)
	{
		if (element->hasSpawned())
		{
			element->draw(renderer);
		}
	}
}

void Snake::update()
{
	if (segments.size()==0 || route->getControlPoints()->size() <= 1)
	{
		return;
	}

	std::list<SnakeSegment*>::iterator it = segments.begin();
	std::list<SnakeSegment*>::iterator it_prev;
	
	if ((*it)->getSampleIndex() == route->getSampledPoints()->size() - 1)
	{
		delete (*it);
		segments.pop_front();
		return;
	}

	if (last_unspawned != segments.end())
	{
		if (last_unspawned == segments.begin())
		{
			(*last_unspawned)->spawn(route);
			last_spawned = last_unspawned++;
		}
		else 
		{
			//check spawning condition
			float distance = glm::distance((*last_spawned)->getPosition(), route->getControlPoints()->at(0));
			if (distance >= distanceBetweenSegments)
			{
				(*last_unspawned)->spawn(route);
				last_spawned = last_unspawned++;
			}
		}
	}
	

	glm::vec2 currentPos;
	glm::vec2 nextSamplePos;
	glm::vec2 direction;
	glm::vec2 distanceVec;
	float distance;
	float delta = 10;
	
	while (it != segments.end())
	{
		if (!(*it)->hasSpawned())
		{
			break;
		}

		currentPos = (*it)->getPosition();
		nextSamplePos = route->getSampledPoints()->at((*it)->getSampleIndex() + 1);
		direction = nextSamplePos - currentPos;
		direction = glm::normalize(direction);
		
		if (currentPos.y< nextSamplePos.y + delta && currentPos.y > nextSamplePos.y - delta &&
			currentPos.x< nextSamplePos.x + delta && currentPos.x > nextSamplePos.x - delta)
		{
			(*it)->setSampleIndex((*it)->getSampleIndex() + 1);
		}

		if (it == segments.begin())
		{
			(*it)->setPosition(currentPos.x + velocity * direction.x, currentPos.y + velocity * direction.y);
		}
		else
		{
			it_prev = --it;
			it++;
			distanceVec = (*it)->getPosition() - (*it_prev)->getPosition();
			distance = sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y);
			if (distance > this->distanceBetweenSegments)
			{
				(*it)->setPosition(currentPos.x + velocity * direction.x, currentPos.y + velocity * direction.y);
			}
		}
		it++;
	}


}

std::list<SnakeSegment*>::iterator Snake::findLastUnspawned()
{
	std::list<SnakeSegment*>::iterator it = segments.begin();
	while (it != segments.end())
	{
		if (!(*it)->hasSpawned())
		{
			break;
		}
		it++;
	}
	return it;
}

glm::vec2 Snake::getHeadPos()
{
	std::list<SnakeSegment*>::iterator it = segments.begin();
	return (*it)->getPosition();
}
glm::vec2 Snake::getTailPos()
{
	std::list<SnakeSegment*>::iterator it = segments.end();
	it--;
	return (*it)->getPosition();
}

void Snake::mergeWith(Snake* other)
{
	this->segments.splice(this->segments.end(), other->segments);

	this->last_unspawned = this->findLastUnspawned();
	other->last_unspawned = other->findLastUnspawned();

	this->last_spawned = this->last_unspawned;
	if (this->last_spawned != segments.begin())
		this->last_spawned--;

	other->last_spawned = other->last_unspawned;
	if (other->last_spawned != other->segments.begin())
		other->last_spawned--;
}

Snake* Snake::split(std::list<SnakeSegment*>::iterator iterator)
{
	Snake* newSnake = new Snake();
	newSnake->velocity = this->velocity;
	newSnake->segmentSize = this->segmentSize;
	newSnake->route = this->route;
	newSnake->textures = this->textures;

	std::list<SnakeSegment*> newSegments;
	newSegments.splice(newSegments.begin(), segments, iterator, segments.end());

	newSnake->length = newSegments.size();
	newSnake->segments = newSegments;

	newSnake->last_unspawned = newSnake->findLastUnspawned();
	this->last_unspawned = this->findLastUnspawned();

	this->last_spawned = this->last_unspawned;
	if(this->last_spawned!=segments.begin())
		this->last_spawned--;

	newSnake->last_spawned = newSnake->last_unspawned;
	if(newSnake->last_spawned!=newSnake->segments.begin())
		newSnake->last_spawned--;

	if (newSnake->segments.size() != 0)
	{
		newSnake->headSampleIndex = newSnake->segments.front()->getSampleIndex();
	}
	return newSnake;
}

std::list<SnakeSegment*>::iterator Snake::insert(float x, float y, int type, std::list<SnakeSegment*>::iterator it)
{
	if (type<textures->size())
	{
		SnakeSegment* newSegment = new SnakeSegment(x, y, segmentSize, textures->at(type));
		newSegment->setSampleIndex((*it)->getSampleIndex());
		newSegment->setTag(type);
		segments.insert(it, newSegment);
		it--;

		this->last_unspawned = this->findLastUnspawned();

		this->last_spawned = this->last_unspawned;
		if (this->last_spawned != segments.begin())
			this->last_spawned--;

		return it;
	}
	else
	{
		this->last_unspawned = this->findLastUnspawned();

		this->last_spawned = this->last_unspawned;
		if (this->last_spawned != segments.begin())
			this->last_spawned--;

		return this->segments.end();
	}

}

Texture* Snake::getTexture(int index)
{
	if (textures->size() - 1 >= index)
	{
		return textures->at(index);
	}
	else
	{
		return nullptr;
	}
}

float Snake::distanceBetweenSegments = 3 * 20;