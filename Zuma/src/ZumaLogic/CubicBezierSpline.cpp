#include "CubicBezierSpline.hpp"

int CubicBezierSpline::getNumberOfSegments()
{
	return (int)((controlPoints.size() - 1) / 3);
}

std::vector<glm::vec2>* CubicBezierSpline::getSampledPoints()
{
	return &sampledPoints;
}

glm::vec2 CubicBezierSpline::getValueAt(float t)
{

	if (controlPoints.size() < 4)
		return glm::vec2(0, 0);

	int splineIndex = trunc(t);

	int n = getNumberOfSegments();
	if (splineIndex < n)
	{
		t = t - trunc(t);
	}
	else
	{
		splineIndex--;
		t = 1;
	}

	glm::vec2 t0 = (1 - t) * (1 - t) * (1 - t) * controlPoints[3 * splineIndex + 0];
	glm::vec2 t1 = 3 * (1 - t) * (1 - t) *t* controlPoints[3 * splineIndex + 1];
	glm::vec2 t2 = 3 * (1 - t) *t*t* controlPoints[3 * splineIndex + 2];
	glm::vec2 t3 = t*t*t* controlPoints[3 * splineIndex + 3];
	return t0 + t1 + t2 + t3;
}

void CubicBezierSpline::sampleSpline()
{
	std::vector <glm::vec2> samples;
	float t;
	for (int i = 0; i < getNumberOfSegments(); i++)
	{
		for (int j = 0 ; j < samples_per_segment + 1; j++)
		{
			t = i + j * (1 / (samples_per_segment + static_cast<float>(1)));
			samples.push_back(getValueAt(t));
		}
	}
	samples.push_back(getValueAt(getNumberOfSegments()));

	sampledPoints = samples;
}

CubicBezierSpline::CubicBezierSpline(float xInit, float yInit, int samples_per_segment, Texture* texture1, Texture* texture2, Texture* segmentTexture, float control_point_size)
{
	this->samples_per_segment = samples_per_segment;
	this->tex1 = texture1;
	this->tex2 = texture2;
	this->control_point_size = control_point_size;
	this->segmentTex = segmentTexture;

	controlPoints.push_back(glm::vec2(xInit,yInit));
}

std::vector<glm::vec2>* CubicBezierSpline::getControlPoints()
{
	return &controlPoints;
}

void CubicBezierSpline::draw(SpriteRenderer* renderer, float width)
{
	if (sampledPoints.empty())
		return;

	if (sampledPoints.size() <2)
		return;

	glm::vec2 pos;
	float distance;
	float theta;
	glm::vec2 direction;
	for (int i = 0; i < sampledPoints.size() - 1; i++)
	{
		pos = 0.5f * (sampledPoints[i] + sampledPoints[i + 1]);
		distance = 0.5*sqrt((sampledPoints[i].x - sampledPoints[i + 1].x) * (sampledPoints[i].x - sampledPoints[i + 1].x) + (sampledPoints[i].y - sampledPoints[i + 1].y) * (sampledPoints[i].y - sampledPoints[i + 1].y));
		direction = sampledPoints[i] - sampledPoints[i + 1];
		theta = atan(direction.y / direction.x)*180/3.14;
		renderer->draw(pos, glm::vec2(distance, width), theta, segmentTex);
	}
}

void CubicBezierSpline::addSegment(float x, float y)
{
	glm::vec2 last = controlPoints.back();
	glm::vec2 direction = glm::vec2(x, y) - last;
	controlPoints.push_back(glm::vec2(last.x + 0.33f * direction.x, last.y + 0.33f * direction.y));
	controlPoints.push_back(glm::vec2(last.x + 0.66f * direction.x, last.y + 0.66f * direction.y));
	controlPoints.push_back(glm::vec2(x, y));

	if (this->controlPoints.size() > 3)
		this->sampleSpline();
}

void CubicBezierSpline::drawControlPoints(SpriteRenderer* renderer)
{
	for (int i = 0; i < controlPoints.size(); i++)
	{
		if (i % 3 == 0)
		{
			renderer->draw(controlPoints[i], glm::vec2(control_point_size, control_point_size), 0, tex1);
		}
		else
		{
			renderer->draw(controlPoints[i], glm::vec2(control_point_size, control_point_size), 0, tex2);
		}
	}
}

bool CubicBezierSpline::removeLastSegment()
{
	if (controlPoints.size() > 3)
	{
		controlPoints.pop_back();
		controlPoints.pop_back();
		controlPoints.pop_back();
		this->sampleSpline();
		return true;
	}
	else
	{
		return false;
	}
}