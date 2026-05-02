#pragma once
#include "CVillain.h"

class CBoss : public CVillain
{
	bool _draw;
public:
	CBoss();
	virtual ~CBoss();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
};