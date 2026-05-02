#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CBox.h"

CBox::CBox() : CShape() 
{
	_vtxCount = 28; _vtxAttrCount = 11; _idxCount = 54;

	_points = new GLfloat[_vtxCount * _vtxAttrCount]{
		// Front face (柔和紅, z = +0.5, 法向量 (0,0,1))
		-0.3f,  -0.5f,  0.3f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  // 0
		 0.3f,  -0.5f,  0.3f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,  // 1
		 0.3f,   0.5f,  0.3f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,  // 2
		-0.3f,   0.5f,  0.3f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  // 3

		-0.074f, -0.15f,  0.3f,   1.0f, 0.6f, 0.6f,   0.0f, 0.0f, -1.0f,   0.3767f, 0.35f,  // 24
		 0.074f, -0.15f,  0.3f,   1.0f, 0.6f, 0.6f,   0.0f, 0.0f, -1.0f,   0.6233f, 0.35f,  // 25
		 0.074f,  0.25f,  0.3f,   1.0f, 0.6f, 0.6f,   0.0f, 0.0f, -1.0f,   0.6233f, 0.75f,  // 26
		-0.074f,  0.25f,  0.3f,   1.0f, 0.6f, 0.6f,   0.0f, 0.0f, -1.0f,   0.3767f, 0.75f,   // 27

		// Back face (柔和綠, z = -0.5, 法向量 (0,0,-1))
		 0.3f, -0.5f, -0.3f,  0.6f, 1.0f, 0.6f,  0.0f, 0.0f, 1.0f,  3.0f, 0.0f,  // 4
		-0.3f, -0.5f, -0.3f,  0.6f, 1.0f, 0.6f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 5
		-0.3f,  0.5f, -0.3f,  0.6f, 1.0f, 0.6f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // 6
		 0.3f,  0.5f, -0.3f,  0.6f, 1.0f, 0.6f,  0.0f, 0.0f, 1.0f,  3.0f, 1.0f,  // 7

		 // Left face (柔和藍, x = -0.5, 法向量 (-1,0,0))
		-0.3f, -0.5f, -0.3f,  0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 8
		-0.3f, -0.5f,  0.3f,  0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,  3.0f, 0.0f,  // 9
		-0.3f,  0.5f,  0.3f,  0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,  3.0f, 1.0f,  // 10
		-0.3f,  0.5f, -0.3f,  0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 11

		// Right face (柔和黃, x = 0.5, 法向量 (1,0,0))
		 0.3f, -0.5f,  0.3f,  1.0f, 1.0f, 0.6f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 12
		 0.3f, -0.5f, -0.3f,  1.0f, 1.0f, 0.6f, -1.0f, 0.0f, 0.0f,  3.0f, 0.0f,  // 13
		 0.3f,  0.5f, -0.3f,  1.0f, 1.0f, 0.6f, -1.0f, 0.0f, 0.0f,  3.0f, 1.0f,  // 14
		 0.3f,  0.5f,  0.3f,  1.0f, 1.0f, 0.6f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 15

		 // Top face (柔和品紅, y = 0.5, 法向量 (0,1,0))
		-0.3f,  0.5f,  0.3f,  1.0f, 0.6f, 1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // 16
		 0.3f,  0.5f,  0.3f,  1.0f, 0.6f, 1.0f,  0.0f, -1.0f, 0.0f,  3.0f, 0.0f,  // 17
		 0.3f,  0.5f, -0.3f,  1.0f, 0.6f, 1.0f,  0.0f, -1.0f, 0.0f,  3.0f, 1.0f,  // 18
		-0.3f,  0.5f, -0.3f,  1.0f, 0.6f, 1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,  // 19

		// Bottom face (柔和青, y = -0.5, 法向量 (0,-1,0))
		-0.3f, -0.5f, -0.3f,  0.6f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // 20
		 0.3f, -0.5f, -0.3f,  0.6f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.1f, 0.0f,  // 21
		 0.3f, -0.5f,  0.3f,  0.6f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.1f, 0.1f,  // 22
		-0.3f, -0.5f,  0.3f,  0.6f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.1f   // 23
	};
	
	_idx = new GLuint[_idxCount]{ 
		// Front face
		0, 1, 5, 0, 5, 4,
		1, 2, 6, 1, 6, 5,
		2, 3, 7, 2, 7, 6,
		3, 0, 4, 3, 4, 7,

		// Back face 
		8, 9, 10, 10, 11, 8,
		// Left face
		12, 13, 14, 14, 15, 12,
		// Right face
		16, 17, 18, 18, 19, 16,
		// Top face
		20, 21, 22, 22, 23, 20,
		// Bottom face
		24, 25, 26, 26, 27, 24,
	};

}

CBox::~CBox()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CBox::draw()
{
	glUseProgram(_shaderProg);
	updateMatrix();
	glBindVertexArray(_vao);
	glUniform1i(_shadingModeLoc, _uShadingMode);
	if ( _bObjColor ) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CBox::drawRaw()
{
	uploadTextureFlags();
	uploadMaterial();
	updateMatrix();
	glBindVertexArray(_vao);
	glUniform1i(_shadingModeLoc, _uShadingMode);
	if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	/*updateMatrix();
	glBindVertexArray(_vao);

	// 傳入 shading mode
	glUniform1i(_shadingModeLoc, _uShadingMode);

	// 如果使用物件顏色，傳入顏色 uniform
	if (_bObjColor) {
		glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	}

	// 如果有貼圖，綁定貼圖並設定 shader uniform
	if (_hasTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textureID);
		GLint texLoc = glGetUniformLocation(_shaderProg, "uTexture");
		glUniform1i(texLoc, 0);  // Texture unit 0
		GLint hasTexLoc = glGetUniformLocation(_shaderProg, "uHasTexture");
		glUniform1i(hasTexLoc, 1);
	}
	else {
		GLint hasTexLoc = glGetUniformLocation(_shaderProg, "uHasTexture");
		glUniform1i(hasTexLoc, 0);
	}

	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/
}


void CBox::update(float dt)
{

}

void CBox::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}

