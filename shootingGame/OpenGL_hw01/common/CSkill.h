#pragma once
#include "CShape.h"
#include "CQuad.h"

class CSkill : public CShape
{
	int _value;
	float _speed;
	float _radius;
	int _category;
	
public:
	CSkill(int category = 1);
	virtual ~CSkill();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
	int getStatus();
	glm::vec3 getPos();
	float getRadius();

	CQuad quad[10];
};