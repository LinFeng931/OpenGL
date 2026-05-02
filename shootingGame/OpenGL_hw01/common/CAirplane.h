#pragma once
#include <glm/glm.hpp>
#include "CShape.h"
#include "CQuad.h"
#include "CDead.h"

//extern struct Villain;

#define QUAD_VTX_COUNT  34
#define QUAD_INDEX_COUNT 55
#define QUAD_VTX_ATTR_COUNT 11	// 單位為 float 個數

class CAirplane : public CShape
{
		float circleRadius;
		bool _isflashing;
		float flashTimer;
		bool _flashOn;
		float nextTime;
		int _blood;
		int normal_blood;
		float _deadtime;
		glm::vec3 re_color;
		float bullet_timer;

		int _type;
		int type1_idxCount;
		int type1_idxStart;
		int type2_idxCount;
		int type2_idxStart;
		int type3_idxCount;
		int type3_idxStart;
		int type4_idxCount;
		int type4_idxStart;
		float _skillTimer;
		bool _skillOn;
		int re_type;

	public:
		CAirplane();
		virtual ~CAirplane();
		virtual void draw() override;
		virtual void reset() override;
		virtual void update(float dt) override;
		bool checkCollision(glm::vec3 pos, float otherRadius);

		glm::vec3 getPos();
		float getPosX();
		float getPosY();

		void Flashing();
		void Inter(float dt);
		void Hart();
		int getBlood();
		void setType(int type);
		int getType();
		void resetTimer();
		float getTimer();
		int getStatus();

		CDead dead;
		CQuad blood[6];
};