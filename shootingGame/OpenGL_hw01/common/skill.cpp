#include "initshader.h"
#include "wmhandler.h"
#include "CShaderPool.h"
#include "CAirplane.h"
#include "CSkill.h"
#include "skill.h"
#define	AMOUNT_SKILL 3

extern GLuint g_shaderProg;
extern CAirplane g_airplane;

CSkill Skill[AMOUNT_SKILL]{ CSkill(1), CSkill(2), CSkill(3) };
int now_skill = 0;
float skill_timer = 0.0f;
bool set_skill = false;

void skill() {
	for (int i = 0; i < AMOUNT_SKILL; i++) {
		Skill[i].setupVertexAttributes();
		Skill[i].setShaderID(g_shaderProg);
		Skill[i].setPos(glm::vec3(rand() % 7 - 3.0f, 6.0f, 0.0f));
	}	
}

void skill_draw() {
	Skill[now_skill].draw();
}

void skill_update(float dt) {
	skill_timer += dt;
	
	if (skill_timer >= 5.0f && set_skill == false) {
		set_skill = true;
		Skill[now_skill].setPos(glm::vec3(rand() % 7 - 3.0f, 6.0f, 0.0f));
	}
	if (set_skill) {
		Skill[now_skill].update(dt);
		if (g_airplane.checkCollision(Skill[now_skill].getPos(), Skill[now_skill].getRadius())) {
			g_airplane.setType(Skill[now_skill].getStatus());

			Skill[now_skill].setPos(glm::vec3(rand() % 7 - 3.0f, 6.0f, 0.0f));
			set_skill = false;
			skill_timer = 0.0f;
			now_skill = (now_skill + 1) % AMOUNT_SKILL;
		}
		if (Skill[now_skill].getPos().y <= -6.0f) {
			Skill[now_skill].setPos(glm::vec3(rand() % 7 - 3.0f, 6.0f, 0.0f));
			set_skill = false;
			skill_timer = 0.0f;
			now_skill = (now_skill + 1) % AMOUNT_SKILL;
		}
	}
}