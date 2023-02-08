#pragma once
#include <list>
#include <glm/vec2.hpp>
#include <cmath>
#include <vector>
#include "../drawables/SpriteRenderer.hpp"

class BezierCubicSpline
{
public:
	BezierCubicSpline(float xInit, float yInit, int samples_per_segment, Texture* texture1, Texture* texture2, Texture* segmentTexture, float control_point_size);
	int getNumberOfSegments();
	glm::vec2 getValueAt(float t);
	std::vector<glm::vec2>* getSampledPoints();
	std::vector<glm::vec2>* getControlPoints();
	void sampleSpline();
	void draw(SpriteRenderer* renderer, float width);
	void drawControlPoints(SpriteRenderer* renderer);
	void addSegment(float x, float y);
	bool removeLastSegment();
private:
	std::vector<glm::vec2> controlPoints;
	std::vector<glm::vec2> sampledPoints;
	int samples_per_segment;
	float control_point_size;
	Texture* tex1;
	Texture* tex2;
	Texture* segmentTex;
};