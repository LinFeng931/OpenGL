#pragma once
#include "CShape.h"

class CBulletThree : public CShape
{
	float circleRadius;
	float circleScale;
	bool _ismove;
	float _speed;
	float _timer;
	float _width;
	float _high;
	float _shock;
	float _scaleTimer;
	int status;

public:
	CBulletThree();
	virtual ~CBulletThree();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;

	glm::vec3 getPos();
	bool checkCollision(glm::vec3 pos, float otherRadius);
	float getRadius();
	int getStatus();
	void setStatus(int s);
};