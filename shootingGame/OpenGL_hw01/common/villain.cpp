#include "initshader.h"
#include "wmhandler.h"
#include "CShaderPool.h"
#include "CBulletOne.h"
#include "CVillain.h"
#include "CAirplane.h"
#include "CShield.h"
#include "CVillainTwo.h"
#include "CBoss.h"
#include "CLevel.h"

extern GLuint g_shaderProg;
extern CAirplane g_airplane;
extern CShield player_shield;
extern CLevel header;

#define	ONEV_AMOUNT 4
#define TWOV_AMOUNT 3

struct Level {
	int times_amount;//船艦一次出來的數量
	int alive_amount; //當前level剩餘的敵人
	int re_amount;	//已死亡的敵人
	int v_index;	//陣列頭

	bool isDeadHandled[5]; //記錄死亡狀態
	bool v_ok[5];
	bool bullet_start[5];
	bool b_ok[5];

	CVillain *villain[5];
};

Level* v_pointer = NULL;

float v_bullet_timer;
float turn_level_timer = 0.0f;
int level = 0;
int re_level = level;

void Villain() { //初始化

	if (v_pointer == NULL) {
		v_pointer = new Level;
		v_pointer->times_amount = 2; 
		v_pointer->v_index = 0;
		v_pointer->re_amount = 0; //已經被打敗多少船艦
		v_pointer->alive_amount = ONEV_AMOUNT;

		v_pointer->villain[0] = new CVillain();
		v_pointer->villain[1] = new CVillain();
		v_pointer->villain[2] = new CVillainTwo();
		v_pointer->villain[3] = new CVillainTwo();
		v_pointer->villain[4] = new CBoss();

		for (int i = 0; i < 5; i++) {
			v_pointer->bullet_start[i] = false;
			v_pointer->b_ok[i] = false;
			v_pointer->v_ok[i] = false;
			v_pointer->isDeadHandled[i] = false;

			//villain
			v_pointer->villain[i]->setupVertexAttributes();
			v_pointer->villain[i]->setShaderID(g_shaderProg);
			v_pointer->villain[i]->setPos(glm::vec3(rand() % 7 - 3.0f, 6.0f, 0.0f));
			if(i != 4) v_pointer->villain[i]->setColor(glm::vec3(rand() % 10 * 0.1f + 0.5f, rand() % 10 * 0.1f + 0.5f, rand() % 10 * 0.1f + 0.5f));

			if (i > 0) {	//確認初始位置不重疊
				if(v_pointer->villain[i-1]->getPos() == v_pointer->villain[i]->getPos())
					v_pointer->villain[i]->setPos(glm::vec3(rand() % 7 - 3.0f, 6.0f, 0.0f));
			}

			//死亡效果
			v_pointer->villain[i]->dead.setupVertexAttributes();
			v_pointer->villain[i]->dead.setShaderID(g_shaderProg);
			if (i == 4) {
				v_pointer->villain[i]->bulletthree.setupVertexAttributes();
				v_pointer->villain[i]->bulletthree.setShaderID(g_shaderProg);
			}
			//子彈
			for (int j = 0; j < 2; j++) {
				v_pointer->villain[i]->bullet[j].setupVertexAttributes();
				v_pointer->villain[i]->bullet[j].setShaderID(g_shaderProg);
				v_pointer->villain[i]->bullet[j].setPos(glm::vec3(5.0f, 5.0f, 0.0f));
				v_pointer->villain[i]->bullet[j].setColor(glm::vec3(1.0f, 1.0f, 0.0f));
				v_pointer->villain[i]->bullet[j].setCategory(1);

				v_pointer->villain[i]->gun[j].setupVertexAttributes();
				v_pointer->villain[i]->gun[j].setShaderID(g_shaderProg);
				v_pointer->villain[i]->gun[j].setPos(glm::vec3(8.0f, 5.0f, 0.0f));

				for (int r = 0; r < 3; r++) {
					v_pointer->villain[i]->gun[j].bullet[r].setupVertexAttributes();
					v_pointer->villain[i]->gun[j].bullet[r].setShaderID(g_shaderProg);
					v_pointer->villain[i]->gun[j].bullet[r].setPos(glm::vec3(5.0f, 5.0f, 0.0f));
				}

				int gun_blood = v_pointer->villain[i]->gun[j].getBlood();
				for (int r = 0; r < gun_blood; r++) {
					v_pointer->villain[i]->gun[j].blood[r].setupVertexAttributes();
					v_pointer->villain[i]->gun[j].blood[r].setShaderID(g_shaderProg);
					v_pointer->villain[i]->gun[j].blood[r].setPos(glm::vec3(5.0f, 5.0f, 0.0f));
				}
			}
			for (int j = 0; j < 3; j++) {
				v_pointer->villain[i]->bullettwo[j].setupVertexAttributes();
				v_pointer->villain[i]->bullettwo[j].setShaderID(g_shaderProg);
				v_pointer->villain[i]->bullettwo[j].setPos(glm::vec3(5.0f, 5.0f, 0.0f));
				v_pointer->villain[i]->bullettwo[j].setColor(glm::vec3(1.0f, 1.0f, 0.0f));
				v_pointer->villain[i]->bullettwo[j].setPath(j + 1);
			}

			for (int j = 0; j < v_pointer->villain[i]->getBlood(); j++) {
				v_pointer->villain[i]->blood[j].setupVertexAttributes();
				v_pointer->villain[i]->blood[j].setShaderID(g_shaderProg);
				v_pointer->villain[i]->blood[j].setPos(glm::vec3(-0.5f - (i * 0.5f), 5.0f, 0.0f));
			}
		}
	}
}

void v_draw() {
		for (int i = v_pointer->v_index; i < (v_pointer->times_amount+v_pointer->v_index); i++) {
			if (v_pointer->villain[i]->getStatus() != -1) {
				if (v_pointer->re_amount < ONEV_AMOUNT) { //level-1
					if (v_pointer->bullet_start[i] == true && v_pointer->villain[i]->getStatus() >= 0) v_pointer->villain[i]->bullet[1].draw();
					v_pointer->villain[i]->draw();
				}	
				else if (v_pointer->re_amount < ONEV_AMOUNT + TWOV_AMOUNT) { //level-2
					if (v_pointer->bullet_start[i] == true && v_pointer->villain[i]->getStatus() >= 0) {
						for (int j = 0; j < 3; j++) {
							v_pointer->villain[i]->bullettwo[j].draw();
						}
					}
					v_pointer->villain[i]->draw();
				}
				else { //level-3
					v_pointer->villain[i]->draw();
					if (v_pointer->bullet_start[i] == true && v_pointer->villain[i]->getStatus() >= 0) {
						for (int j = 0; j < 2; j++) {
							if (v_pointer->villain[i]->gun[j].getStatus() != -1) {
								for (int r = 0; r < 3; r++) {
									v_pointer->villain[i]->gun[j].bullet[r].draw();
								}
							}
						}
						for (int j = 0; j < 2; j++) {
							v_pointer->villain[i]->bullet[j].draw();
						}
					}
					for (int j = 0; j < 2; j++) {
						v_pointer->villain[i]->gun[j].draw();
					}
				}
			}
		}
}

bool header_ok = false;
void v_update(float dt) {
	v_bullet_timer += dt;

	if (level == 0) level = 1;
	if (re_level != level) {
		turn_level_timer += dt;
		if (header_ok == false) {
			header.HeaderOn(level);
			header_ok = true;
		}
		if (turn_level_timer >= 5.0f) {
			header_ok = false;
			re_level = level;
			turn_level_timer = 0.0f;
		}
	}
	for (int i = v_pointer->v_index; i < (v_pointer->times_amount+v_pointer->v_index) && re_level == level; i++) {
		if (v_pointer->villain[i]->getPos().y <= 4.0f) v_pointer->v_ok[i] = true; //villain就定位
		if (v_pointer->villain[i]->getStatus() != -1) {	//villain是否死亡
			v_pointer->villain[i]->update(dt);

			if (v_pointer->bullet_start[i] == false && v_bullet_timer >= 0.5f && v_pointer->v_ok[i] == true) {	//子彈時間計算
				v_pointer->bullet_start[i] = true;
			}
			if (v_pointer->bullet_start[i]) {
				if (v_pointer->b_ok[i] == false) {
					if (v_pointer->re_amount < ONEV_AMOUNT) v_pointer->villain[i]->bullet[1].setPos(v_pointer->villain[i]->getPos());
					else if (v_pointer->re_amount < ONEV_AMOUNT + TWOV_AMOUNT) {
						for (int j = 0; j < 3; j++) {
							v_pointer->villain[i]->bullettwo[j].setPos(v_pointer->villain[i]->getPos());
						}
					}
					else {
						if (v_pointer->villain[i]->gun[0].getStatus() == -1 && v_pointer->villain[i]->gun[1].getStatus() == -1) {
							for (int j = 0; j < 2; j++) {
								v_pointer->villain[i]->bullet[j].setPos(glm::vec3(v_pointer->villain[i]->getPos().x + 0.925f - (1.85f * j), v_pointer->villain[i]->getPos().y, 0.0f));
							}
						}
						else {
							for (int j = 0; j < 2; j++) {
								for (int r = 0; r < 3; r++) {
									v_pointer->villain[i]->gun[j].bullet[r].setPos(v_pointer->villain[i]->gun[j].getPos());
								}
							}
						}
					}
					v_pointer->b_ok[i] = true;
				}
				if (v_pointer->re_amount < ONEV_AMOUNT && v_pointer->v_ok[i]) v_pointer->villain[i]->bullet[1].update(dt);
				else if (v_pointer->re_amount < ONEV_AMOUNT + TWOV_AMOUNT && v_pointer->v_ok[i]) {
					for (int j = 0; j < 3; j++) {
						v_pointer->villain[i]->bullettwo[j].update(dt);
					}
				}
				else {
					if (v_pointer->villain[i]->gun[0].getStatus() == -1 && v_pointer->villain[i]->gun[1].getStatus() == -1) {
						for (int j = 0; j < 2; j++) {
							v_pointer->villain[i]->bullet[j].update(dt);
						}
					}
					else {
						for (int j = 0; j < 2; j++) {
							for (int r = 0; r < 3; r++) {
								v_pointer->villain[i]->gun[j].bullet[r].update(dt);
							}
							v_pointer->villain[i]->gun[j].update(dt);
						}
					}
				}

				if (v_pointer->re_amount < ONEV_AMOUNT) {
					if (v_pointer->villain[i]->bullet[1].getPos().y <= -6.0f || v_pointer->villain[i]->getStatus() <= 0) {	//子彈重設
						v_bullet_timer = 0.0f;
						v_pointer->bullet_start[i] = false;
						v_pointer->b_ok[i] = false;
					}
				}
				else if (v_pointer->re_amount < ONEV_AMOUNT + TWOV_AMOUNT) {
					for (int j = 0; j < 3; j++) {
						if (v_pointer->villain[i]->bullettwo[j].getPos().y <= -6.0f || v_pointer->villain[i]->getStatus() <= 0) {	//子彈重設
							v_bullet_timer = 0.0f;
							v_pointer->bullet_start[i] = false;
							v_pointer->b_ok[i] = false;
						}
					}
				}
				else {
					if (v_pointer->villain[i]->gun[0].getStatus() == -1 && v_pointer->villain[i]->gun[1].getStatus() == -1) {
						for (int j = 0; j < 2; j++) {
							if (v_pointer->villain[i]->bullet[j].getPos().y <= -6.0f || v_pointer->villain[i]->getStatus() <= 0) {	//子彈重設
								v_bullet_timer = 0.0f;
								v_pointer->bullet_start[i] = false;
								v_pointer->b_ok[i] = false;
							}
						}
					}
					else {
						for (int j = 0; j < 2; j++) {
							for (int r = 0; r < 3; r++) {
								if (v_pointer->villain[i]->gun[j].bullet[r].getPos().y <= -6.0f || v_pointer->villain[i]->getStatus() <= 0) {	//子彈重設
									v_bullet_timer = 0.0f;
									v_pointer->bullet_start[i] = false;
									v_pointer->b_ok[i] = false;
								}
							}
						}
						//}
					}
				}

				//碰撞設定
				if (v_pointer->re_amount < ONEV_AMOUNT) {
					if (g_airplane.checkCollision(v_pointer->villain[i]->bullet[1].getPos(), v_pointer->villain[i]->bullet[1].getRadius())) {
						if (player_shield.getStatus() == 2) {
							player_shield.Flashing();
						}
						else {
							g_airplane.Hart();
							g_airplane.Flashing();
						}
						v_pointer->villain[i]->bullet[1].setPos(v_pointer->villain[i]->getPos());
						v_bullet_timer = 0.0f;
						v_pointer->bullet_start[i] = false;
					}
				}
				else if (v_pointer->re_amount < ONEV_AMOUNT + TWOV_AMOUNT) {
					for (int j = 0; j < 3; j++) {
						if (g_airplane.checkCollision(v_pointer->villain[i]->bullettwo[j].getPos(), v_pointer->villain[i]->bullettwo[j].getRadius())) {
							if (player_shield.getStatus() == 2) {
								player_shield.Flashing();
							}
							else {
								g_airplane.Hart();
								g_airplane.Flashing();
							}
							v_pointer->villain[i]->bullettwo[j].setPos(glm::vec3(10.0f, v_pointer->villain[i]->bullettwo[j].getPos().y, 0.0f));
						}
					}
				}
				else {
					if (v_pointer->villain[i]->bulletthree.getStatus() == 0) {
						if (g_airplane.checkCollision(v_pointer->villain[i]->bulletthree.getPos(), v_pointer->villain[i]->bulletthree.getRadius())) {
							if (player_shield.getStatus() == 2) {
								player_shield.Flashing();
							}
							else {
								g_airplane.Hart();
								g_airplane.Flashing();
							}
						}
					}
					if (v_pointer->villain[i]->gun[0].getStatus() == -1 && v_pointer->villain[i]->gun[1].getStatus() == -1) {
						for (int j = 0; j < 2; j++) {
							if (g_airplane.checkCollision(v_pointer->villain[i]->bullet[j].getPos(), v_pointer->villain[i]->bullet[j].getRadius())) {
								if (player_shield.getStatus() == 2) {
									player_shield.Flashing();
								}
								else {
									g_airplane.Hart();
									g_airplane.Flashing();
								}
								v_pointer->villain[i]->bullet[j].setPos(glm::vec3(10.0f, v_pointer->villain[i]->bullet[j].getPos().y, 0.0f));
							}
						}
					}
					else {
						for (int j = 0; j < 2; j++) {
							if (v_pointer->villain[i]->gun[j].getStatus() != 0) {
								for (int r = 0; r < 3; r++) {
									if (g_airplane.checkCollision(v_pointer->villain[i]->gun[j].bullet[r].getPos(), v_pointer->villain[i]->gun[j].bullet[r].getRadius())) {
										if (player_shield.getStatus() == 2) {
											player_shield.Flashing();
										}
										else {
											g_airplane.Hart();
											g_airplane.Flashing();
										}
										v_pointer->villain[i]->gun[j].bullet[r].setPos(glm::vec3(10.0f, v_pointer->villain[i]->gun[j].bullet[r].getPos().y, 0.0f));
									}
								}
							}
						}
					}
				}
			}

			//敵人死亡重設
			else if (v_pointer->villain[i]->getStatus() == -1 && v_pointer->isDeadHandled[i] != true) {
				v_pointer->re_amount++;
				v_pointer->alive_amount--;
				std::cout << v_pointer->re_amount << " , " << v_pointer->alive_amount << std::endl; //debug
				v_pointer->isDeadHandled[i] = true;
				v_pointer->villain[i]->setPos(glm::vec3(0.0f, 10.0f, 0.0f));

				if (v_pointer->re_amount + 1 < ONEV_AMOUNT && level == 1) {
					v_pointer->isDeadHandled[i] = false;
					v_pointer->villain[i]->reset();
					v_pointer->villain[i]->setPos(glm::vec3(rand() % 7 - 3.0f, 4.5f, 0.0f));
				}
				else if (v_pointer->re_amount + 1 < TWOV_AMOUNT + ONEV_AMOUNT && level == 2) {
					v_pointer->isDeadHandled[i] = false;
					v_pointer->villain[i]->reset();
					v_pointer->villain[i]->setPos(glm::vec3(rand() % 7 - 3.0f, 4.5f, 0.0f));
				}

				if (v_pointer->re_amount == ONEV_AMOUNT && v_pointer->alive_amount == 0) {
					level++;
					v_pointer->v_index += v_pointer->times_amount;
					v_pointer->alive_amount = TWOV_AMOUNT;
					for (int r = v_pointer->v_index; r < (v_pointer->times_amount + v_pointer->v_index); r++) {
						v_pointer->villain[r]->reset();
						v_pointer->villain[r]->setPos(glm::vec3(rand() % 7 - 3.0f, 5.0f, 0.0f));
					}
				}
				else if (v_pointer->re_amount == ONEV_AMOUNT + TWOV_AMOUNT && v_pointer->alive_amount == 0) {
					level++;
					v_pointer->v_index += v_pointer->times_amount;
					v_pointer->times_amount = 1;
					v_pointer->alive_amount = 1;
					for (int r = v_pointer->v_index; r < (v_pointer->times_amount+v_pointer->v_index); r++) {
						v_pointer->villain[r]->reset();
						v_pointer->villain[r]->setPos(glm::vec3(rand() % 7 - 3.0f, 5.0f, 0.0f));
					}
				}
				else if (i == 4 && header.getType() != 4 && g_airplane.getStatus() != 0){
					header.HeaderOn(4);
				}
			}
		}
	}
}

int v_getAmount() {
	return(v_pointer->times_amount);
}
CVillain& v_get(int t) {
	return *v_pointer->villain[t];
}

int v_getIndex() {
	return(v_pointer->v_index);
}

bool v_OK(int t) {
	return(v_pointer->v_ok[t]);
}
void v_release(){
	for (int i = 0; i < 5; i++) {
		delete v_pointer->villain[i];
	}
	delete v_pointer;
}
