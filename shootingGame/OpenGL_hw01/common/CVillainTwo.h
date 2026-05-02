#pragma once
#include "CVillain.h"

class CVillainTwo : public CVillain
{
	float g_angle;
public:
	CVillainTwo();
	virtual ~CVillainTwo();
	virtual void reset() override;
	virtual void update(float dt) override;
};