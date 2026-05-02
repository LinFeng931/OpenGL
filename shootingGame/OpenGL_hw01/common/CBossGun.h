#pragma once
#include "CShape.h"
#include "CBulletTwo.h"
#include "CQuad.h"

class CBossGun : public CShape
{
	float g_angle;
	float g_type;
	glm::vec3 re_color;
	int _blood;
	bool _isflashing;
	float flashTimer;
	bool _flashOn;
	float nextTime;

	float circleRadius;

public:
	CBossGun(int type = 1);
	virtual ~CBossGun();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
	bool checkCollision(glm::vec3 pos, float otherRadius);
	glm::vec3 getPos();
	void Flashing();
	int getStatus();
	int getBlood();

	CBulletTwo bullet[3];
	CQuad blood[4];
};