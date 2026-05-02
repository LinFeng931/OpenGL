#pragma once
#include "CShape.h"
#include "CQuad.h"

class CShield : public CShape
{
	float g_angle;
	int status = 2;
	int _blood;
	bool _isflashing;
	float flashTimer;
	bool _flashOn;
	float nextTime;
	float _timer;
	float blood_timer;

	int type1_idxCount;
	int type2_idxCount; 
	int type2_idxStart;

public:
	CShield();
	virtual ~CShield();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
	void setStatus(int s);
	int getStatus();
	void Flashing();
	int getBlood();
	glm::vec3 getPos();
	
	CQuad blood[3];
};