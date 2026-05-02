#include <iostream>
#include <glew/include/GL/glew.h>
#include <glfw/include/GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "../models/CBullet.h"
#include "../models/CObj.h"
#include "../models/CQuad.h"
#include "../models/CRoom.h"
#include "../models/CCube.h"
#define BULLET_NUM 20

CBullet bullet[BULLET_NUM];
int num = 0;
extern GLuint g_nolightShader;
extern CObj
g_doll[6][2],
g_chair,
g_table,
g_bookcase,
g_vase,
g_plant,
g_target[3],
g_door[6][2];

extern CQuad hole[10];
extern CRoom room[6];
extern CCube break_cube[50];
extern glm::vec3 g_eyeloc;

int hole_count = 0;

void shoot(glm::vec3 pos, glm::vec3 front) {
	bullet[num].setPos(pos);
	bullet[num].setFront(front);
	bullet[num].setisMove(true);

	if (num < 19)num++;
	else num = 0;
}

void bullet_update(float dt) {
	for (int i = 0; i < BULLET_NUM; i++) {
		bullet[i].update(dt);

		//obj碰撞偵測
		for (int j = 0; j < 3; j++) {
			if (g_target[j].Collision(bullet[i].getPos(), 1.0f) && bullet[i].getisMove()) {
				bullet[i].setisMove(false);
				g_target[j].Break();
			}
			if (g_target[j].isBreak() && g_target[j].breakFinial() == false) {
				if (bullet[i].getisMove() == false) g_target[j].setFinial();
				for (int r = 0; r < 50; r++) {
					break_cube[r].BreakAct(g_target[j].getPos());
				}
			}
		}

		if (g_doll[0][0].Collision(bullet[i].getPos(), 1.0f) && bullet[i].getisMove()) {
			bullet[i].setisMove(false);
			g_doll[0][0].Break();
			g_doll[0][1].Break();
		}
		if (g_doll[0][0].isBreak() && g_doll[0][0].breakFinial() == false) {
			if (bullet[i].getisMove() == false) {
				g_doll[0][0].setFinial();
				g_doll[0][1].setFinial();
			}
			for (int r = 0; r < 50; r++) {
				break_cube[r].BreakAct(g_doll[0][0].getPos());
			}
		}

		//牆壁碰撞偵測
		glm::vec3 bullet_pos = bullet[i].getPos();
		bool bullet_active = bullet[i].getisMove();

		//橫向偵測
		if (bullet_pos.z > -9.0f && bullet_active && room[0].Collisionx(bullet_pos.x, bullet_pos.z, 1, g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[0].getCollisionWall(); //-1右邊，1左邊

			if(hole_count < 9){
				hole[hole_count].setRotate(90.0f, glm::vec3(0,1,0));
				hole[hole_count].setPos(glm::vec3(-8.8f*wall, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room0" << std::endl;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-8.8f * wall, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room0" << std::endl;
			}
			room[0].resetCollision();
		}
		if (bullet_pos.z > -27.0f && bullet_pos.z <= -9.0f && bullet_active && room[1].Collisionx(bullet_pos.x, bullet_pos.z, 1, g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[1].getCollisionWall(); //-1右邊，1左邊
			if (hole_count < 9) {
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-8.8f * wall, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room1" << std::endl;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-8.8f * wall, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room1" << std::endl;
			}
			room[1].resetCollision();
		}
		if (bullet_pos.z > -45.0f && bullet_pos.z <= -27.0f && bullet_active && room[2].Collisionx(bullet_pos.x, bullet_pos.z, 1, g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[2].getCollisionWall(); //-1右邊，1左邊
			if (hole_count < 9) {
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-8.8f * wall, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room2" << std::endl;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-8.8f * wall, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room2" << std::endl;
			}
			room[2].resetCollision();
		}
		if (bullet_pos.z > -9.0f && bullet_active && room[3].Collisionx(bullet_pos.x, bullet_pos.z, 1, g_door[4][0].getPos().x, g_door[4][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[3].getCollisionWall() -1; //-1右邊，1左邊
			if (hole_count < 9) {
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-26.7f - float(wall)*8.6f , bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room3" << std::endl;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-26.7f - float(wall) * 8.6f, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room3" << std::endl;
			}
			room[3].resetCollision();
		}
		if (bullet_pos.z > -27.0f && bullet_pos.z <= -9.0f && bullet_active && room[4].Collisionx(bullet_pos.x, bullet_pos.z, 1, g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[4].getCollisionWall() -1; //-1右邊，1左邊
			if (hole_count < 9) {
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-26.7f - float(wall) * 8.6f, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room4" << std::endl;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-26.7f - float(wall) * 8.6f, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room4" << std::endl;
			}
			room[4].resetCollision();
		}
		if (bullet_pos.z > -45.0f && bullet_pos.z <= -27.0f && bullet_active && room[5].Collisionx(bullet_pos.x, bullet_pos.z, 1, g_door[5][0].getPos().x, g_door[5][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[5].getCollisionWall() -1; //-1右邊，1左邊
			if (hole_count < 9) {
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-26.7f - float(wall) * 8.6f, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room5" << std::endl;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(90.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(-26.7f - float(wall) * 8.6f, bullet_pos.y, bullet_pos.z));
				hole[hole_count].setHoleActive(true);
				hole_count++;
				std::cout << hole_count << ", room5" << std::endl;
			}
			room[5].resetCollision();
		}

		//豎向
		if (bullet_pos.x > -9.0f && bullet_active && room[0].Collisionz(bullet_pos.x, bullet_pos.z, 1, g_door[0][0].getPos().x, g_door[0][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[0].getCollisionWall(); //-1後，1前

			if (hole_count < 9) {
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, 8.8f* float(wall)));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, 8.8f * float(wall)));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			room[0].resetCollision();
		}
		if (bullet_pos.x > -9.0f && bullet_active && room[1].Collisionz(bullet_pos.x, bullet_pos.z, 1, g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[1].getCollisionWall() + 1; //-1後，1前
			if (hole_count < 9) {
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, -26.7f + float(wall)*8.6f));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, -26.7f + float(wall) * 8.6f));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			room[1].resetCollision();
		}
		if (bullet_pos.x > -9.0f && bullet_active && room[2].Collisionz(bullet_pos.x, bullet_pos.z, 1, g_door[1][0].getPos().x, g_door[1][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[2].getCollisionWall() + 1; //-1右邊，1左邊
			if (hole_count < 9) {
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, -44.7f + float(wall) * 8.6f));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, -44.7f + float(wall) * 8.6f));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			room[2].resetCollision();
		}
		if (bullet_pos.x <= -9.0f && bullet_active && room[3].Collisionz(bullet_pos.x, bullet_pos.z, 1, g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[3].getCollisionWall(); //-1右邊，1左邊
			if (hole_count < 9) {
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, 8.8f * float(wall)));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, 8.8f * float(wall)));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			room[3].resetCollision();
		}
		if (bullet_pos.x <= -9.0f && bullet_active && room[4].Collisionz(bullet_pos.x, bullet_pos.z, 1, g_door[3][0].getPos().x, g_door[3][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[4].getCollisionWall()+1; //-1右邊，1左邊
			if (hole_count < 9) {
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, -26.7f + float(wall) * 8.6f));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, -26.7f + float(wall) * 8.6f));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			room[4].resetCollision();
		}
		if (bullet_pos.x <= -9.0f && bullet_active && room[5].Collisionz(bullet_pos.x, bullet_pos.z, 1, g_door[2][0].getPos().x, g_door[2][0].getPos().z, 0.4f)) {
			bullet[i].setisMove(false);
			int wall = room[5].getCollisionWall()+1; //-1右邊，1左邊
			if (hole_count < 9) {
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, -44.7f + float(wall) * 8.6f));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			else {
				hole_count = 0;
				hole[hole_count].setHoleActive(false);
				hole[hole_count].setRotate(0.0f, glm::vec3(0, 1, 0));
				hole[hole_count].setPos(glm::vec3(bullet_pos.x, bullet_pos.y, -44.7f + float(wall) * 8.6f));
				hole[hole_count].setHoleActive(true);
				hole_count++;
			}
			room[5].resetCollision();
		}
	}
}

void bullet_draw(glm::mat4 mxView, glm::mat4 mxProj) {
	glUseProgram(g_nolightShader); // 使用 shader program
	glUniformMatrix4fv(glGetUniformLocation(g_nolightShader, "mxView"), 1, GL_FALSE, glm::value_ptr(mxView));
	glUniformMatrix4fv(glGetUniformLocation(g_nolightShader, "mxProj"), 1, GL_FALSE, glm::value_ptr(mxProj));

	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullet[i].getisMove()) {
			// model matrix（根據子彈的位置）
			glm::mat4 model = bullet[i].getModelMatrix();
			glUniformMatrix4fv(glGetUniformLocation(g_nolightShader, "mxModel"), 1, GL_FALSE, glm::value_ptr(model));

			//bullet[i].drawRaw();
		}
	}
}

void bullet_load() {
	for (int i = 0; i < BULLET_NUM; i++) {
		bullet[i].setupVertexAttributes();
		bullet[i].setShaderID(g_nolightShader);
	}
}