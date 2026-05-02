#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CBulletOne.h"

CBulletOne::CBulletOne(int type) : CShape()
{
	_type = type;

	_vtxCount = 4; _vtxAttrCount = 11; _idxCount = 6;

	_points = new GLfloat[_vtxCount * _vtxAttrCount]{
		// 位置               // 顏色                     // 法向量       // 貼圖座標
		-0.00835f, -0.125f, 0.0f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 左下
		 0.00835f, -0.125f, 0.0f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // 右下
		 0.00835f,  0.125f, 0.0f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 右上
		-0.00835f,  0.125f, 0.0f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // 左上
	};

	_idx = new GLuint[_idxCount]{ 
		0, 1, 2, 2, 3, 0, 
	};

	circleRadius = 0.1f; //碰撞圓的初始大小設定，位置為_pos
	circleScale = 1.0f;
	_speed = 4.0f;
	_ismove = true;
	_category = 0; //0 = player, 1 = villain

	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件
}

CBulletOne::~CBulletOne()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CBulletOne::draw()
{
	glUseProgram(_shaderProg);
	updateMatrix();
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
}

void CBulletOne::update(float dt)
{
	if (_type == 2) {
		if (_ismove && _category == 0) {
			direction = glm::normalize(v_pos - _pos);
			setRotZ(glm::degrees(atan2(direction.x, direction.y)));
		
			_pos += direction * _speed * dt;
			glm::vec3 next_pos = _pos + direction * _speed * dt;

			if (glm::distance(next_pos, v_pos) < glm::distance(_pos, v_pos) ) setPos(_pos);
			else { 
				_pos = v_pos;
				_ismove = false;
			}
			
			if (_pos.y >= 10.0f) _ismove = false;
		}
	}
	else {
		if (_ismove && _category == 0) {
			setPos(glm::vec3(_pos.x, _pos.y+_speed * dt, 0.0f));
			if (_pos.y >= 10.0f) _ismove = false;
		}
		else if (_category == 1) {
			setPos(glm::vec3(_pos.x, _pos.y-_speed *dt, 0.0f));
		}
	}
}

void CBulletOne::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}

glm::vec3 CBulletOne::getPos() {
	return(_pos);
}

float CBulletOne::getRadius() {
	return(circleRadius);
}

void CBulletOne::setisMoving() {
	_ismove = !_ismove;
}

void CBulletOne::setCategory(int cate) {
	_category = cate;
}

bool CBulletOne::checkCollision(glm::vec3 pos, float otherRadius) {
	float dx = pos[0] - _pos.x;
	float dy = pos[1] - _pos.y;
	float combineRadius = circleRadius + otherRadius;
	if ((dx * dx + dy * dy) <= (combineRadius * combineRadius) || v_pos == _pos) return(true);
	else return(false);
}

void CBulletOne::setType(int type) {
	_type = type;
}
int CBulletOne::getType() {
	return(_type);
}
void CBulletOne::cathcPos(glm::vec3 pos) {
	v_pos = pos;
}