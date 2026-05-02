#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CBullet.h"

CBullet::CBullet() : CShape()
{
	_isMove = false;
	_vtxCount = 24; _vtxAttrCount = 19; _idxCount = 36;

	_points = new GLfloat[_vtxCount * _vtxAttrCount]{
		// Front face
		-0.05f, -0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		 0.05f, -0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 1
		 0.05f,  0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 2
		-0.05f,  0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		// Back face
		 0.05f, -0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 4
		-0.05f, -0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 5
		-0.05f,  0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 6
		 0.05f,  0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 7

		 // Left face 
		-0.05f, -0.05f, -0.35f,  0.0f, 1.0f, 0.8f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 8
		-0.05f, -0.05f,  0.35f,  0.0f, 1.0f, 0.8f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 9
		-0.05f,  0.05f,  0.35f,  0.0f, 1.0f, 0.8f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 10
		-0.05f,  0.05f, -0.35f,  0.0f, 1.0f, 0.8f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 11

		// Right face
		 0.05f, -0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 12
		 0.05f, -0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 13
		 0.05f,  0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 14
		 0.05f,  0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 15

		 // Top face
		-0.05f,  0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 16
		 0.05f,  0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 17
		 0.05f,  0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 18
		-0.05f,  0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 19

		// Bottom face
		-0.05f, -0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 20
		 0.05f, -0.05f, -0.35f,  0.0f, 1.0f, 0.8f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 21
		 0.05f, -0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 22
		-0.05f, -0.05f,  0.35f,  0.0f, 1.0f, 0.8f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 23
	};

	_idx = new GLuint[_idxCount]{
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
	};

	_clock = 0.0f;
	_front = glm::vec3(0.0f);
	_pos = glm::vec3(0.0f);
}

CBullet::~CBullet()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CBullet::draw()
{
	if (_isMove) {
		glUseProgram(_shaderProg);
		updateMatrix();
		glBindVertexArray(_vao);
		glUniform1i(_shadingModeLoc, _uShadingMode);
		if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
		glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void CBullet::drawRaw()
{
	uploadTextureFlags();
	uploadMaterial();
	updateMatrix();
	glBindVertexArray(_vao);
	glUniform1i(_shadingModeLoc, _uShadingMode);
	if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CBullet::update(float dt)
{
	if (_isMove) {
		float speed = 15.0f;
		_pos += _front * speed * dt;
		setPos(_pos);

		// --- 計算與設定朝向 ---
		glm::vec3 targetDir = glm::normalize(_front);
		glm::vec3 defaultDir = glm::vec3(0, 0, 1); // 模型預設朝 Z+

		glm::vec3 rotationAxis = glm::cross(defaultDir, targetDir);
		float cosTheta = glm::dot(defaultDir, targetDir);
		float angle = acos(glm::clamp(cosTheta, -1.0f, 1.0f)); // 弧度

		// 特例：完全相同方向或反方向
		if (glm::length(rotationAxis) < 0.0001f) {
			if (cosTheta > 0.999f) {
				angle = 0.0f;
				rotationAxis = glm::vec3(0, 1, 0); // 任意軸
			}
			else {
				angle = glm::pi<float>();          // 180 度
				rotationAxis = glm::vec3(0, 1, 0); // 可選 X 或 Y 軸
			}
		}

		setRotate(glm::degrees(angle), rotationAxis); // 注意 setRotate 要角度
	}
}

void CBullet::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}

void CBullet::setisMove(bool ismove) {
	_isMove = ismove;
}

void CBullet::setFront(glm::vec3 front) {
	_front = front;
}

bool CBullet::getisMove() {
	return(_isMove);
}

glm::vec3 CBullet::getDir() {
	return(_front);
}