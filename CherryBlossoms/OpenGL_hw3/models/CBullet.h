#pragma once
#include "CShape.h"
#include "..\common\CMaterial.h"

class CBullet : public CShape
{
private:
	float _clock;
	glm::vec3 _front;
	bool _isMove;

public:
	CBullet();
	virtual ~CBullet();
	virtual void draw() override;
	virtual void drawRaw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
	void setisMove(bool ismove);
	void setFront(glm::vec3 front);
	bool getisMove();
	glm::vec3 getDir();
};