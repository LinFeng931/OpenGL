#pragma once
#include "CShape.h"
#include "CBulletOne.h"
#include "CBulletTwo.h"
#include "CDead.h"
#include "CQuad.h"
#include "CBossGun.h"
#include "CBulletThree.h"

class CVillain : public CShape
{
protected:
	int _type; 
	float circleRadius;
	float circleScale;
	bool _isflashing;
	float flashTimer;
	bool _flashOn;
	float nextTime;
	float _speed;
	int _blood;
	float _deadtime;

	bool PosX;
	bool PosY;
	bool startPos;
	float moving_timer;
	glm::vec3 re_color;

	int type1_idxCount;
	int type2_idxCount; 
	int type2_idxStart;
	int type3_idxCount;
	int type3_idxStart;
	
public:
	CVillain(int type = 1);
	virtual ~CVillain();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
	bool checkCollision(glm::vec3 pos, float otherRadius);
	glm::vec3 getPos();
	void Flashing();
	void Inter(float dt);
	void Moving(float dt);
	int getStatus();
	int getBlood();

	
	CDead dead;
	CQuad blood[10];
	CBulletOne bullet[2];
	CBulletTwo bullettwo[3];
	CBossGun gun[2]{CBossGun(1),CBossGun(2)};
	CBulletThree bulletthree;
};