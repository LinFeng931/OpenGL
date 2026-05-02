#pragma once
#include "CShape.h"

class CDead : public CShape
{
	float timer;
public:
	CDead();
	virtual ~CDead();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
};