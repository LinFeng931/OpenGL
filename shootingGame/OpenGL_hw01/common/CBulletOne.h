#pragma once
#include "CShape.h"

class CBulletOne : public CShape
{
	float circleRadius;
	float circleScale;
	bool _ismove;
	float _speed;
	int _category;
	int _type; //1 = normal, 2 = °lÂÜ¼u
	glm::vec3 type2_pos;
	glm::vec3 v_pos;
	glm::vec3 direction;

public:
	CBulletOne(int type = 1);
	virtual ~CBulletOne();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;

	glm::vec3 getPos();
	bool checkCollision(glm::vec3 pos, float otherRadius);
	float getRadius();
	void setisMoving();
	void setCategory(int cate);
	void setType(int type);
	int getType();
	void cathcPos(glm::vec3 pos);
};