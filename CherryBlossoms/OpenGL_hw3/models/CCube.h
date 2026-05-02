#pragma once
#include "CShape.h"
#include "..\common\CMaterial.h"

class CCube : public CShape
{
private:
	float _clock;
	bool  _bautoRotate;
	float _radius;
	float _acttype;
	float _breakclock;
	float _breakflag;
	glm::vec3 _direction;

public:
	CCube();
	virtual ~CCube();
	virtual void draw() override;
	virtual void drawRaw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
	void setAutoRotate();
	void setActType(int type);
	void BreakAct(glm::vec3 pos);
};