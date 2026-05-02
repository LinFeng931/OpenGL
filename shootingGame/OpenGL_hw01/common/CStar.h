#pragma once
#include "CShape.h"

class CStar : public CShape
{
public:
	CStar();
	virtual ~CStar();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
	glm::vec3 getPos();
};