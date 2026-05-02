#pragma once
#include "CShape.h"

class CBulletTwo : public CShape
{
	float g_angle;
	int _path;
	float _speed;
	float circleRadius;

public:
	CBulletTwo();
	virtual ~CBulletTwo();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
	glm::vec3 getPos();
	void setPath(int path);
	float getRadius();
};