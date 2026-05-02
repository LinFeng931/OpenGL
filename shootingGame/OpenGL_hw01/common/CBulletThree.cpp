#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CBulletThree.h"

CBulletThree::CBulletThree() : CShape()
{
	_vtxCount = 4; _vtxAttrCount = 11; _idxCount = 6;

	_points = new GLfloat[_vtxCount * _vtxAttrCount]{
		// 位置               // 顏色                     // 法向量       // 貼圖座標
		-0.01f, -0.125f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 左下
		 0.01f, -0.125f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // 右下
		 0.01f,  0.125f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 右上
		-0.01f,  0.125f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f  // 左上
	};

	_idx = new GLuint[_idxCount]{ 0, 1, 2, 2, 3, 0 };
	_speed = 60.0f;
	circleRadius = 1.0f; //碰撞圓的初始大小設定，位置為_pos
	circleScale = 1.0f;
	_timer = 0.0f;
	_scaleTimer = 0.0f;
	_width = 1.0f;
	_high = 1.0f;
	_shock = -1.0f;
	status = -1;
	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件
}

CBulletThree::~CBulletThree()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CBulletThree::draw()
{
	glUseProgram(_shaderProg);
	updateMatrix();
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
}

void CBulletThree::update(float dt)
{
	_timer += dt;
	_scaleTimer += dt;
	if (_timer <= 1.0f) {
		setScale(glm::vec3(1.0f, _speed * _scaleTimer, 1.0f));
		_high = _speed * _scaleTimer;
	}
	else if (_timer <= 1.5f) {
		_scaleTimer = 0.0f;
	}
	else if (_timer < 2.0f) {
		setScale(glm::vec3(3.0f*_speed * _scaleTimer, _high, 1.0f));
		_width = 3.0f*_speed * _scaleTimer;
	}
	else if (_timer <= 4.0f) {
		_scaleTimer = 0.0f;
		setScale(glm::vec3(_width + _shock, _high, 1.0f));
		_shock *= -1.0f;
	}
	else {
		if (_width > 0.0f) {
			_width -= 5.0f * _speed * dt;
			setScale(glm::vec3(_width, _high, 1.0f));
		}
		else status = 1;
	}
	
	
	//else {
		//setScale(glm::vec3(0.0001 * dt, 1.0f, 1.0f));
		//setPos(glm::vec3(_pos.x + 5.0f * dt, _pos.y, 0.0f));
	//}
}

void CBulletThree::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}

glm::vec3 CBulletThree::getPos() {
	return(glm::vec3(_pos.x, -3.0f, 0.0f));
}

float CBulletThree::getRadius() {
	return(circleRadius);
}

bool CBulletThree::checkCollision(glm::vec3 pos, float otherRadius) {
	float dx = pos[0] - _pos.x;
	float dy = pos[1] - _pos.y;
	float combineRadius = circleRadius + otherRadius;
	if ((dx * dx + dy * dy) <= (combineRadius * combineRadius)) return(true);
	else return(false);
}

int CBulletThree::getStatus() {
	return(status);
}

void CBulletThree::setStatus(int s) {
	status = s;
}