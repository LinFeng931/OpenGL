#pragma once
#include "CShape.h"
#include "..\common\CMaterial.h"
#include <glm/glm.hpp>
#include "CQuad.h"
#include "CObj.h"
#include "CCube.h"

class CRoom : public CShape
{
private:
	float _wall_x1, _wall_x2;
	float _wall_z1, _wall_z2;
	bool hr1 = false, hr2 = false, di1 = false, di2 = false; //¸I¼²¨ì­þ¤@­±Àð
	float _distance;

public:
	CRoom(float wallx1, float wallx2, float wallz1, float wallz2);
	virtual ~CRoom();
	virtual void draw() override;
	virtual void drawRaw() override;
	virtual void reset() override;
	virtual void update(float dt) override;

	int Collisionx(float posx, float posz, bool doorOpen, float doorposx, float doorposz, float distance); //µL¸I¼²0, ¼²¥k1, ¼²¥ª
	int Collisionz(float posx, float posz, bool doorOpen, float doorposx, float doorposz, float distance);
	int getCollisionWall();
	void resetCollision();

	CQuad wall[4], ground, top;
	CCube pillair[8];
};