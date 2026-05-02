#pragma once
#include "CShape.h"

class CQuad : public CShape
{
	int _walltype;
	bool _holeActive = false;
	float _holeclock;

public:
	CQuad();
	virtual ~CQuad();
	virtual void draw() override;
	virtual void drawRaw() override;
	virtual void reset() override;
	virtual void update(float dt) override;

	void setWallType(int type);
	int getType();
	void setHoleActive(bool flag);
	bool getHoleActive();
};