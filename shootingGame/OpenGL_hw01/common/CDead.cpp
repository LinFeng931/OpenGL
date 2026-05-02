#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CDead.h"

CDead::CDead() : CShape()
{
	_vtxCount = 16; _vtxAttrCount = 11; _idxCount = 24;

	_points = new GLfloat[_vtxCount * _vtxAttrCount]{
		// 位置            // 顏色         // 法向量       // 貼圖座標
		-0.01f,  0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
		 0.0f,  0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
		 0.0f,  0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
		-0.01f,  0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

		-0.2f, -0.005f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
		-0.1f,  -0.005f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
		-0.1f,  0.005f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
		 -0.2f,  0.005f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上

		 -0.01f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
		  0.0f,  -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
		  0.0f,  -0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
		 -0.01f, -0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f , // 左上

		0.1f, -0.005f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
		0.2f,  -0.005f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 右下
		 0.2f,  0.005f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 右上
		 0.1f,  0.005f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上
	};
	_idx = new GLuint[_idxCount]{ 
		0, 1, 2, 2, 3, 0,  // 前端
		4, 5, 6, 6, 7, 4,
		8, 9 ,10, 10, 11, 8,
		12, 13, 14, 14, 15, 12
	};

	timer = 0.0f;
	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件
}

CDead::~CDead()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CDead::draw()
{
	if (timer <= 1.0f) {
		glUseProgram(_shaderProg);
		updateMatrix();
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	}	
}

void CDead::update(float dt)
{
	timer += dt;
	if (timer <= 0.6f) setScale(glm::vec3(2.0f * timer));
}

void CDead::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
	timer = 0.0f;
}