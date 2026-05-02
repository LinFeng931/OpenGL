#pragma once
#include "CShape.h"

class CLevel : public CShape
{
	int _type;
	int type1_idxCount;
	int type2_idxCount;
	int type3_idxCount;
	int type2_idxStart;
	int type3_idxStart;
	int type0_idxCount;
	int type0_idxStart;
	int type4_idxCount; 
	int type4_idxStart;

	float _timer;
	bool _active;
	bool _isdraw;

public:
	CLevel();
	virtual ~CLevel();
	virtual void draw() override;
	virtual void reset() override;
	virtual void update(float dt) override;
	void HeaderOn(int level);
	int getType();
};