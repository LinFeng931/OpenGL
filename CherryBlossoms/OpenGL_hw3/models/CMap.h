#pragma once
#include "CShape.h"
#include "..\common\CMaterial.h"
#include <glm/glm.hpp>
#include "CQuad.h"
#include "CObj.h"
#include "CCube.h"

class CMap : public CShape
{
private:
	float _wall_x1, _wall_x2;
	float _wall_z1, _wall_z2;

public:
	CMap();
	virtual ~CMap();
	virtual void draw() override;
	virtual void drawRaw() override;
	virtual void reset() override;
	virtual void update(float dt) override;

	CQuad wall[4], ground, top;
};