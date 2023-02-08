#include "SnakeManager.hpp"

SnakeManager::SnakeManager(Shooter* shooter, Snake* initialSnake)
{
	this->shooter = shooter;
	this->snakes.push_back(initialSnake);
}

void SnakeManager::push_back_snake(Snake* snake)
{
	snakes.push_back(snake);
}

void SnakeManager::push_front_snake(Snake* snake)
{
	snakes.push_front(snake);
}

void SnakeManager::checkForHits()
{
	std::vector<Shot*>::iterator it = shooter->getProjectiles()->begin();
	bool hit = false;

	

	while (it != shooter->getProjectiles()->end())
	{
		std::list<Snake*>::iterator it_snake = snakes.begin();
		while(it_snake!=snakes.end())
		{
			std::list<SnakeSegment*>::iterator it_seg = (*it_snake)->getSegments()->begin();
			if (it_seg == (*it_snake)->getSegments()->end())
			{
				return;
			}
			while (it_seg != (*it_snake)->getSegments()->end())
			{
				//scored a hit
				if ((*it_seg)->checkCollision((*it)))
				{
					std::list<SnakeSegment*>::iterator it_new = (*it_snake)->insert((*it)->getPosition().x, (*it)->getPosition().y, (*it)->getTag(), it_seg);

					if (this->checkPopingCondition(it_new, it_snake, 3))
					{
						this->popSame(it_new, it_snake);
					}
					shooter->getProjectiles()->erase(it);
					return;
				}
				it_seg++;
			}
			it_snake++;
		}
		it++;
	}
}

void SnakeManager::update(GLFWwindow* window)
{
	updatePos();
	checkForHits();
	shooter->updateProjectiles(window);
}

void SnakeManager::updatePos()
{
	if (snakes.size() == 0)
	{
		return;
	}
	std::list<Snake*>::iterator it_pusher = snakes.begin();

		if ((*it_pusher)->getSegments()->size() == 0)
		{
			snakes.erase(snakes.begin());
			return;
		}

		if (snakes.size() > 1)
		{
			std::list<Snake*>::iterator it_next = it_pusher;
			it_next++;
			if ((*it_next)->getSegments()->size() == 0)
			{
				snakes.erase(it_next);
				return;
			}
			glm::vec2 difference=(*it_next)->getTailPos()-(*it_pusher)->getHeadPos();

			if (sqrt(difference.x * difference.x + difference.y * difference.y) <= Snake::getDeistanceBetweenSegments())
			{
				//merge
				(*it_next)->getSegments()->splice((*it_next)->getSegments()->end(), *((*it_pusher)->getSegments()));
				snakes.erase(it_pusher);
				return;
			}
		}

		(*it_pusher)->update();
}

void SnakeManager::draw(SpriteRenderer* renderer)
{
	shooter->drawProjectiles(renderer);
	for (auto snake : snakes)
	{
		snake->draw(renderer);
	}
	shooter->drawProjectiles(renderer);
}

bool SnakeManager::checkPopingCondition(std::list<SnakeSegment*>::iterator it, std::list<Snake*>::iterator it_snake, int count)
{
	std::list<SnakeSegment*>::iterator it_forward, it_reverse;
	it_forward = it;
	it_reverse = it;
	it_forward++;
	int tag = (*it)->getTag();
	int n = 0;

	while (it_forward != (*it_snake)->getSegments()->end())
	{
		if ((*it_forward)->getTag() == tag)
		{
			n++;
			it_forward++;
		}
		else
		{
			break;
		}
	}
	if (it_reverse == (*it_snake)->getSegments()->begin() && (*it_reverse)->getTag() == tag)
	{
		n++;
	}
	else
	{
		while (it_reverse != (*it_snake)->getSegments()->begin())
		{
			if ((*it_reverse)->getTag() == tag)
			{
				n++;
				it_reverse--;
			}
			else
			{
				break;
			}
		}
	}
	
	if (n >= count)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SnakeManager::popSame(std::list<SnakeSegment*>::iterator it_segment, std::list<Snake*>::iterator it_snake)
{
	std::list<SnakeSegment*>::iterator it_reverse=it_segment;
	std::list<SnakeSegment*>::iterator it_forward=it_segment;
	it_forward++;
	int tag = (*it_segment)->getTag();

	while (it_forward != (*it_snake)->getSegments()->end())
	{
		if ((*it_forward)->getTag() == tag)
		{
			(*it_snake)->getSegments()->erase(it_forward++);
		}
		else
		{
			break;
		}
	}
	while(it_reverse != (*it_snake)->getSegments()->begin())
	{
		if ((*it_reverse)->getTag() == tag)
		{
			(*it_snake)->getSegments()->erase(it_reverse--);
		}
		else
		{
			break;
		}
	}
	if (it_reverse == (*it_snake)->getSegments()->begin() && (*it_reverse)->getTag() == tag)
	{
		(*it_snake)->getSegments()->erase(it_reverse++);
	}
	snakes.insert(it_snake, (*it_snake)->split(it_forward));
}

bool SnakeManager::replaceShooter(Shooter* shooter)
{
	if (shooter != nullptr)
	{
		delete shooter;
		this->shooter = shooter;
		return true;
	}
	else
	{
		return false;
	}
}

bool SnakeManager::replaceInitialSnake(Snake* snake)
{
	if (snakes.size() == 1 && snake!=nullptr)
	{
		std::list<Snake*>::iterator it = snakes.begin();
		delete *it;
		snakes.erase(snakes.begin());
		snakes.push_back(snake);
		return true;
	}
	else if (snakes.size() == 0)
	{
		snakes.push_back(snake);
		return true;
	}
	else
	{
		return false;
	}
}