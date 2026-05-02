#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CShape.h"
#include "CShield.h"

CShield::CShield() : CShape()
{
	_vtxCount = 48;
	_vtxAttrCount = 11;
	_idxCount = 108;

	type1_idxCount = 72;
	type2_idxCount = 36; type2_idxStart = 72;

	_points = new GLfloat[_vtxCount * _vtxAttrCount]{
			// 外層六邊形（原始）
	 0.0f,       0.75f,      0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.0f,
	 0.6495f,    0.375f,     0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.75f,
	 0.6495f,   -0.375f,     0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.25f,
	 0.0f,      -0.75f,      0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.0f,
	-0.6495f,   -0.375f,     0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.25f,
	-0.6495f,    0.375f,     0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.75f,

	// 內層六邊形（黑色）
	 0.0f,       0.75f,    0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.75f,
	 0.627375f,  0.3625f,  0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.75f, 0.625f,
	 0.6495f, -0.375f,     0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.75f, 0.375f,
	 0.0f,      -0.725f,   0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.25f,
	-0.6495f, -0.375f,     0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.375f,
	-0.627375f,  0.3625f,  0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.625f,


	// 外層六邊形（旋轉 30 度）
	 0.375f,     0.6495f,    0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.0f,
	 0.75f,      0.0f,       0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.75f,
	 0.375f,    -0.6495f,    0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.25f,
	-0.375f,    -0.6495f,    0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.0f,
	-0.75f,      0.0f,       0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.25f,
	-0.375f,     0.6495f,    0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.75f,

	// 內層六邊形（旋轉 30 度，黑色）
	 0.3625f,    0.627375f,   0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.75f,
	 0.75f,     0.0f,        0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.75f, 0.625f,
	 0.3625f,   -0.627375f,   0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.75f, 0.375f,
	-0.375f,   -0.6495f,   0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.25f,
	-0.725f,     0.0f,        0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.375f,
	-0.375f,    0.6495f,   0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.625f,


	//type2 方形6 --------------------------------------------

		0.0075f, -0.03f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0525f, -0.03f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0525f,  0.06f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0075f,  0.06f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		-0.0525f, -0.03f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.0075f, -0.03f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.0075f,  0.06f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.0525f,  0.06f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		0.0075f, -0.045f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0375f, -0.045f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0375f, -0.03f,  0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0075f, -0.03f,  0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		-0.0375f, -0.045f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.0075f, -0.045f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.0075f, -0.03f,  0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.0375f, -0.03f,  0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		-0.0225f, -0.06f,  0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0225f, -0.06f,  0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.0225f, -0.045f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.0225f, -0.045f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		-0.0075f,  0.045f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0075f,  0.045f, 0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.0075f,  0.06f,  0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.0075f,  0.06f,  0.0f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	};

	_idx = new GLuint[_idxCount]{
		// 原始六邊形（0~11）
		0, 1, 7,  0, 7, 6,
		1, 2, 8,  1, 8, 7,
		2, 3, 9,  2, 9, 8,
		3, 4,10,  3,10, 9,
		4, 5,11,  4,11,10,
		5, 0, 6,  5, 6,11,

		// 旋轉後六邊形（12~23）
		12,13,19,  12,19,18,
		13,14,20,  13,20,19,
		14,15,21,  14,21,20,
		15,16,22,  15,22,21,
		16,17,23,  16,23,22,
		17,12,18,  17,18,23,

		//type2
		24, 25, 26, 26, 27, 24,
		28, 29, 30, 30, 31, 28,
		32, 33, 34, 34, 35, 32,
		36, 37, 38, 38, 39, 36,
		40, 41, 42, 42, 43, 40,
		44, 45, 46, 46, 47, 44,
	};
	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件

	g_angle = 0.0f;
	_blood = 3;
	flashTimer = 0.0f;
	nextTime = 0.0f;
	_timer = 0.0f;
	_flashOn = false;
	_isflashing = false;
	status = 1;
	blood_timer = 0.0f;
}

CShield::~CShield()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CShield::draw()
{
	if (status == 2) {
		for (int j = 0; j < _blood; j++) {
			blood[j].setColor(glm::vec3(0.0f, 1.0f, 0.8f));
			blood[j].draw();
		}
		glUseProgram(_shaderProg);
		updateMatrix();
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, type1_idxCount, GL_UNSIGNED_INT, 0);
	}
	else if (status == 1) {
		for (int j = 0; j < _blood; j++) {
			blood[j].setColor(glm::vec3(0.0f, 1.0f, 0.8f));
			blood[j].setScale(glm::vec3(0.5f, 0.3f, 1.0f));
			blood[j].draw();
		}
		glUseProgram(_shaderProg);
		updateMatrix();
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, type2_idxCount, GL_UNSIGNED_INT, (void*)(type2_idxStart * sizeof(GLuint)));
	}
}

void CShield::update(float dt)
{
	if (status == 2) {
		//_mxTransform = glm::mat4(1.0f);
		_timer += dt;
		blood_timer += dt;
		if (_timer >= 6.0f || _blood == 0) setStatus(0);
		if (blood_timer >= 2.0f && _blood > 0) {
			blood_timer = 0.0f;
			_blood--;
		}

		for (int i = 0; i < _blood; i++) {
			blood[i].setScale(glm::vec3(1.0f, 0.3f, 1.0f));
			blood[i].setPos(glm::vec3(_pos.x + 0.1f * i, _pos.y - 0.7f, 0.0f));
			blood[i].setPos(glm::vec3(blood[i].getPos().x - 0.1425f, blood[i].getPos().y +1.5f, 0.0f));
		}

		g_angle += 90.0f * dt;
		if (g_angle > 360.0f) g_angle -= 360.0f;
		setRotZ(g_angle);

		if (_isflashing && _blood > 0) {
			flashTimer += dt;
			if (flashTimer >= nextTime) {
				_flashOn = !_flashOn;
				if (_flashOn) setColor(glm::vec3(0.0f));
				else setColor(glm::vec3(1.0f));
				nextTime += 0.1f;
			}
		}
		if (nextTime > 0.5f) {
			_isflashing = false;
			flashTimer = 0.0f;
			nextTime = 0.0f;
			setColor(glm::vec3(1.0f));
		}
	}
	if (status == 1) {
		_timer += dt;
		if (_timer >= 1.0f) {
			if(_blood < 3) _blood++;
			_timer = 0.0f;
		}
	}
	if (status == 0) {
		_timer += dt;
		if (_timer >= 2.0f) status = 1;
	}
}

void CShield::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
	g_angle = 0.0f;
	_blood = 3;
	flashTimer = 0.0f;
	nextTime = 0.0f;
	_flashOn = false;
	_isflashing = false;
}

void CShield::setStatus(int s) {
	_timer = 0.0f;
	blood_timer = 0.0f;
	if (s == 2) {
		_bOnTransform = _bTransform = false;
		_mxRotation = glm::mat4(1.0f);
		_mxTransform = glm::mat4(1.0f);
		_mxFinal = glm::mat4(1.0f);
		for (int i = 0; i < 3; i++) {
			blood[i].reset();
		}
		status = 2;
	}
	else if (s == 1) {
		_bOnTransform = _bTransform = false;
		_mxRotation = glm::mat4(1.0f);
		_mxTransform = glm::mat4(1.0f);
		_mxFinal = glm::mat4(1.0f);
		for (int i = 0; i < 3; i++) {
			blood[i].reset();
		}
		status = 1;
		_blood = 1;
	}
	else {
		_mxRotation = glm::mat4(1.0f);
		_mxTransform = glm::mat4(1.0f);
		_mxFinal = glm::mat4(1.0f);
		for (int i = 0; i < 3; i++) {
			blood[i].reset();
		}
		status = 0;
		_blood = 0;
	}
}
int CShield::getStatus() {
	return(status);
}

void CShield::Flashing() {
	_isflashing = true;
}

int CShield::getBlood() {
	return(_blood);
}

glm::vec3 CShield::getPos() {
	return(_pos);
}