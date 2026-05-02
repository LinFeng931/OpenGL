#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CQuad.h"

CQuad::CQuad() : CShape() 
{
	_walltype = 0;
	_vtxCount = 60; _vtxAttrCount = 19; _idxCount = 90;

	_points = new GLfloat[_vtxCount * _vtxAttrCount]{
		// 位置            // 顏色            // 法向量          // 貼圖座標 //light map  //tangent        //bitangent
		-0.5f, -0.5f, 0.0f, 0.5f, 0.7f, 0.4f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 左下
		 0.5f, -0.5f, 0.0f, 0.5f, 0.7f, 0.4f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 右下
		 0.5f,  0.5f, 0.0f, 0.5f, 0.7f, 0.4f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 右上
		-0.5f,  0.5f, 0.0f, 0.5f, 0.7f, 0.4f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 左上

		//type2 有門
		-0.5f,   0.23f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		 0.5f,   0.23f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 1
		 0.5f,   0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.5f, 0.1f,  1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 2
		-0.5f,   0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.1f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		-0.5f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 0
		-0.12f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 1
		-0.12f,   0.23f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 2
		-0.5f,   0.23f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 3

		 0.5f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 0
		 0.12f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 1
		 0.12f,   0.23f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 2
		 0.5f,   0.23f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 3

		//type3有窗
		-0.5f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		 0.5f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 1
		 0.5f,  0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f, 0.9f, 1.0f,  1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 2
		-0.5f,  0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		-0.5f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 0
		 0.5f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.9f, 0.0f,  1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 1
		 0.5f,  -0.05f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.9f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 2
		-0.5f,  -0.05f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 3

		-0.5f,  -0.05f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		-0.13f,  -0.05f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 0.0f,1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 1
		-0.13f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 2
		-0.5f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		0.5f,  -0.05f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		0.13f,  -0.05f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 0.0f,1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 1
		0.13f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 2
		0.5f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		//type4有大窗
		-0.5f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		 0.5f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 1
		 0.5f,  0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.9f, 1.0f,  1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 2
		-0.5f,  0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		-0.5f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		-0.27f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 0.0f,1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 1
		-0.27f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 2
		-0.5f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		0.5f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		0.27f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 0.0f,1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 1
		0.27f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 2
		0.5f,  0.22f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		//type5中窗
		-0.5f,  0.27f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		 0.5f,  0.27f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 1
		 0.5f,  0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f, 0.9f, 1.0f,  1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 2
		-0.5f,  0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		-0.5f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 0
		 0.5f,  -0.5f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.9f, 0.0f,  1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,// 1
		 0.5f,  -0.11f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.9f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 2
		-0.5f,  -0.11f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 3

		-0.5f,  -0.11f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		-0.13f,  -0.11f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 0.0f,1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 1
		-0.13f,  0.3f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 2
		-0.5f,  0.3f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3

		0.5f,  -0.11f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 0
		0.13f,  -0.11f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 0.0f,1.0f, 0.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  // 1
		0.13f,  0.3f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.6f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 2
		0.5f,  0.3f,  0.0f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, // 3
	};
	_idx = new GLuint[_idxCount]{ 
		0, 1, 2, 2, 3, 0,

		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11,8,
		12, 13, 14, 14, 15, 12,

		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20,
		24, 25, 26, 26, 27, 24,
		28, 29, 30, 30, 31, 28,

		32, 33, 34, 34, 35, 32,
		36, 37, 38, 38, 39, 36,
		40, 41, 42, 42, 43, 40,

		44, 45, 46, 46, 47, 44,
		48, 49, 50, 50, 51, 48,
		52, 53, 54, 54, 55, 52,
		56, 57, 58, 58, 59, 56,
	};

	genTangentBitangent();
	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件
}

CQuad::~CQuad()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CQuad::draw()
{
	glUseProgram(_shaderProg);
	glBindVertexArray(_vao);
	updateMatrix();
	glUniform1i(_shadingModeLoc, _uShadingMode);
	if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CQuad::drawRaw()
{
	uploadTextureFlags();
	uploadMaterial();
	updateMatrix();
	glBindVertexArray(_vao);
	glUniform1i(_shadingModeLoc, _uShadingMode);
	if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	if (_walltype == 0) glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (_walltype == 1) glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));
	if (_walltype == 2) glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)));
	if (_walltype == 3) glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (void*)(48 * sizeof(GLuint)));
	if (_walltype == 4) glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)(66 * sizeof(GLuint)));
	glBindVertexArray(0);
}

void CQuad::update(float dt)
{
	if (_holeActive) {
		_holeclock += dt;
		if(_holeclock >= 5.0f){
			_holeclock = 0.0f;
			_holeActive = false;
		}
	}
}

void CQuad::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}

void CQuad::setWallType(int type) {
	_walltype = type;
}

int CQuad::getType() {
	return(_walltype);
}

void CQuad::setHoleActive(bool flag) {
	_holeActive = flag;
	if (flag == false) _holeclock = 0.0f;
}
bool CQuad::getHoleActive() {
	return(_holeActive);
}
