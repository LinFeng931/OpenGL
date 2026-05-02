#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#include "CBossGun.h"
#include "CShaderPool.h"

CBossGun::CBossGun(int type) : CShape()
{
	g_type = type;
	if (g_type == 1) {
		_vtxCount = 40; _vtxAttrCount = 11; _idxCount = 56;

		_points = new GLfloat[_vtxCount * _vtxAttrCount]{
			// 位置            // 顏色         // 法向量       // 貼圖座標

			//type1 8方形----------------------------------
			-0.125f, -1.0f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.125f, -1.0f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.125f,  0.95f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			-0.125f,  0.95f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			-0.075f, -1.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.075f, -1.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.075f,  -1.0f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			-0.075f,  -1.0f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			 0.125f,  0.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.175f,  0.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.175f,  0.25f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			 0.125f,  0.25f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			 0.125f, -0.25f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.175f, -0.25f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.175f, -0.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			 0.125f, -0.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			-0.225f, -0.1f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			-0.125f, -0.1f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			-0.125f,  0.1f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			-0.225f,  0.1f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			-0.175f,  0.7f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			-0.125f,  0.7f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			-0.125f,  0.85f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			-0.175f,  0.85f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			-0.225f,  0.55f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			-0.175f,  0.55f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			-0.175f,  0.7f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			-0.225f,  0.7f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			-0.275f,  0.4f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			-0.225f,  0.4f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			-0.225f,  0.55f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			-0.275f,  0.55f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上
		};
		_idx = new GLuint[_idxCount]{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,

			20, 21, 22, 22, 23, 20,
			24, 25, 26, 26, 27, 24,
			28, 29, 30, 30, 31, 28,
		};
	}

	else if (g_type == 2) {
		_vtxCount = 32; _vtxAttrCount = 11; _idxCount = 48;

		_points = new GLfloat[_vtxCount * _vtxAttrCount]{
			// 位置            // 顏色         // 法向量       // 貼圖座標

			//type1 8方形----------------------------------
			-0.125f, -1.0f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.125f, -1.0f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.125f,  0.95f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			-0.125f,  0.95f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			-0.075f, -1.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.075f, -1.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.075f,  -1.0f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			-0.075f,  -1.0f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			 -0.175f,  0.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 -0.125f,  0.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 -0.125f,  0.25f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			 -0.175f,  0.25f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			 -0.175f, -0.25f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 -0.125f, -0.25f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 -0.125f, -0.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			 -0.175f, -0.1f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			 0.125f, -0.1f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.225f, -0.1f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.225f,  0.1f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			 0.125f,  0.1f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			 0.125f,  0.7f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.175f,  0.7f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.175f,  0.85f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			 0.125f,  0.85f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			 0.175f,  0.55f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.225f,  0.55f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.225f,  0.7f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			 0.175f,  0.7f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

			 0.225f,  0.4f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			 0.275f,  0.4f, 0.0f,  0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
			 0.275f,  0.55f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
			 0.225f,  0.55f, 0.0f, 0.9f, 0.9f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上
		};
		_idx = new GLuint[_idxCount]{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,

			20, 21, 22, 22, 23, 20,
			24, 25, 26, 26, 27, 24,
			28, 29, 30, 30, 31, 28,
		};
	}

	g_angle = 0.0f;
	re_color = glm::vec3(0.9f, 0.9f, 0.9f);
	_blood = 4;
	circleRadius = 0.6f;
	for (int i = 0; i < 3; i++) {
		bullet[i].setPath(i+1);
	}
	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件
}

CBossGun::~CBossGun()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CBossGun::draw()
{
	if (_blood > 0) {
		glUseProgram(_shaderProg);
		updateMatrix();
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	}
	for (int j = 0; j < _blood; j++) {
		blood[j].draw();
	}
}

void CBossGun::update(float dt)
{
	for (int i = 0; i < _blood; i++) {
		blood[i].setPos(glm::vec3(_pos.x + 0.1f * i, _pos.y, 0.0f));
		blood[i].setPos(glm::vec3(blood[i].getPos().x - 0.225f, blood[i].getPos().y, 0.0f));
	}

	if (getStatus() == 0) {
		_blood--;
	}

	if (_isflashing && _blood > 0) {
		flashTimer += dt;
		if (flashTimer >= nextTime) {
			_flashOn = !_flashOn;
			if (_flashOn) setColor(glm::vec3(0.0f));
			else setColor(glm::vec3(re_color));
			nextTime += 0.1f;
		}
	}
	if (nextTime > 0.5f) {
		_isflashing = false;
		flashTimer = 0.0f;
		nextTime = 0.0f;
		setColor(glm::vec3(re_color));
	}
}

bool CBossGun::checkCollision(glm::vec3 pos, float otherRadius) {
	float dx = pos[0] - _pos.x;
	float dy = pos[1] - _pos.y;
	float combineRadius = circleRadius + otherRadius;
	if ((dx * dx + dy * dy) <= (combineRadius * combineRadius) && _blood > 0) {
		_blood--;
		return(true);
	}
	else return(false);
}

glm::vec3 CBossGun::getPos() {
	return(glm::vec3(_pos.x, _pos.y - 0.9f, _pos.z));
}
void CBossGun::Flashing() {
	_isflashing = true;
}

int CBossGun::getStatus() {
	if (_blood < 0) {
		return(-1);
	}
	else if (_blood == 0) {
		return(0);
	}
	else if (_blood <= 1) {
		return(1);
	}
}
int CBossGun::getBlood() {
	return(_blood);
}

void CBossGun::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}