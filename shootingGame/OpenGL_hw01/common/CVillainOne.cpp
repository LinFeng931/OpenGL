#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <time.h>

#include "CVillainOne.h"
#include "initshader.h"


CVillainOne::CVillainOne()
{
	_vao = 0; _vbo = 0; _ebo = 0;
	_shaderProg = 0;
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_color = glm::vec3(1.0f, 1.0f, 1.0f);
	_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	_rotX = 0.0f; _rotY = 0.0f; _rotZ = 0.0f;
	_rotAxis = 0;
	_bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
	_mxScale = glm::mat4(1.0f);
	_mxPos = glm::mat4(1.0f);
	_mxRotX = glm::mat4(1.0f);
	_mxRotY = glm::mat4(1.0f);
	_mxRotZ = glm::mat4(1.0f);
	_mxRotation = glm::mat4(1.0f);
	_mxTRS = glm::mat4(1.0f);
	_mxTransform = glm::mat4(1.0f);
	_mxFinal = glm::mat4(1.0f);
	_points = new GLfloat[QUAD_VTX_COUNT * QUAD_VTX_ATTR_COUNT]{
		// 位置               顏色                 法向量            贴图座標
		// 機身
		 0.0f,  0.4f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f, 0.5f, 1.0f,  // 前
		 0.1f,  0.3f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f, 1.0f, 0.7f,  // 中前右
		-0.1f,  0.3f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f, 0.0f, 0.7f,  // 中前左
		 0.1f, -0.2f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f, 1.0f, 0.3f,  // 中後右
		-0.1f, -0.2f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f, 0.0f, 0.3f,  // 中後左
		 0.0f, -0.4f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f, 0.5f, 0.0f,  // 後

		// 左翼
		-0.1f,  0.2f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  0.0f, 0.0f,  // 右上
		-0.1f, -0.2f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  1.0f, 0.0f,  // 右下
		-0.4f, 0.3f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  0.2f, 1.0f,  //左上
		-0.4f, 0.2f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  1.0f, 1.0f,   //左下

		// 右翼
		0.1f,  0.2f, 0.0f, 0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  0.0f, 0.0f,  // 右上
		0.1f, -0.2f, 0.0f, 0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  1.0f, 0.0f,  // 右下
		0.4f, 0.3f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  0.2f, 1.0f,  //左上
		0.4f, 0.2f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  1.0f, 1.0f,   //左下

		// 左翼(尾)
		0.0f, 0.25f, 0.0f, 0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  0.0f, 0.0f,  // 右上
		0.0f, 0.4f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  1.0f, 0.0f,  // 右下
		-0.2f, 0.4f, 0.0f,0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  0.2f, 1.0f,  //左上
		-0.2f, 0.45f, 0.0f, 0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  1.0f, 1.0f,   //左下

		// 右翼(尾)
		0.0f, 0.25f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  0.0f, 0.0f,  // 右上
		0.0f, 0.4f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  1.0f, 0.0f,  // 右下
		0.2f, 0.4f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  0.2f, 1.0f,  //左上
		0.2f, 0.45f, 0.0f,  0.7f,0.7f,0.7f,  0.0f,0.0f,1.0f,  1.0f, 1.0f,   //左下

		//裝飾(左)
		-0.2f,  0.05f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 右上
		-0.2f, -0.15f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // 右下
		-0.25f,  0.05f, 0.0f, 0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // 左上
		-0.25f, -0.15f, 0.0f, 0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 左下

		//裝飾(右)
		0.2f,  0.05f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 右上
		0.2f, -0.15f, 0.0f,  0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // 右下
		0.25f,  0.05f, 0.0f, 0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // 左上
		0.25f, -0.15f, 0.0f, 0.7f, 0.7f, 0.7f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 左下
	};

	_idx = new GLuint[QUAD_INDEX_COUNT]{
		0, 1, 2,  // 前端
		1, 3, 4,  4, 2, 1,  // 中段
		3, 5, 4,   // 後端
		6, 7, 8, 8, 7, 9,   // 左翼
		10, 11, 12, 12, 11, 13,  // 右翼
		14, 15, 16, 16, 15, 17,
		18, 19, 20, 20, 19, 21,
		22, 23, 24, 24, 23, 25,
		26, 27, 28, 28, 27, 29
	};

	circleRadius = 0.3f; //碰撞圓的初始大小設定，位置為_pos
	circleScale = 1.0f;
	_speed = 20;
	srand(time(NULL));
	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件
}

CVillainOne::~CVillainOne()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	glDeleteProgram(_shaderProg);  //釋放 shader program
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CVillainOne::setupVertexAttributes()
{
	// 設定 VAO、VBO 與 EBO
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(_vao);

	// 設定 VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, QUAD_VTX_COUNT * QUAD_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);

	// 設定 EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, QUAD_INDEX_COUNT * sizeof(GLuint), _idx, GL_STATIC_DRAW);

	// 位置屬性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, QUAD_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	// 顏色屬性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, QUAD_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//法向量屬性
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, QUAD_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//貼圖座標屬性
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, QUAD_VTX_ATTR_COUNT * sizeof(float), BUFFER_OFFSET(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0); // 解除對 VAO 的綁定
}

GLuint CVillainOne::setShader(const char* vshader, const  char* fshader)
{
	_shaderProg = createShader(vshader, fshader);
	glUseProgram(_shaderProg);
	_modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel"); 	// 取得 model matrix 變數的位置
	glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
	return _shaderProg;
}

void CVillainOne::setShaderID(GLuint shaderID)
{
	_shaderProg = shaderID;
	glUseProgram(_shaderProg);
	_modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel"); 	// 取得 MVP 變數的位置
	glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
}

void CVillainOne::setColor(glm::vec3 vColor)
{
	_color = vColor;
	for (int i = 0; i < QUAD_VTX_COUNT; i++) {
		_points[i * QUAD_VTX_ATTR_COUNT + COLOR_OFFSET] = _color.x;
		_points[i * QUAD_VTX_ATTR_COUNT + COLOR_OFFSET + 1] = _color.y;
		_points[i * QUAD_VTX_ATTR_COUNT + COLOR_OFFSET + 2] = _color.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, QUAD_VTX_COUNT * QUAD_VTX_ATTR_COUNT * sizeof(_points), _points, GL_STATIC_DRAW);
}

void CVillainOne::draw()
{
	glUseProgram(_shaderProg);
	updateMatrix();
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, QUAD_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}

void CVillainOne::setScale(glm::vec3 vScale)
{
	_scale = vScale;
	_bScale = true;
	_mxScale = glm::scale(glm::mat4(1.0f), _scale);
}

void CVillainOne::setPos(glm::vec3 vPt)
{
	_pos = vPt;
	_bPos = true;
	_mxPos = glm::translate(glm::mat4(1.0f), _pos);
}

void CVillainOne::setRotX(float angle)
{
	_rotX = glm::radians(angle);
	_rotAxis = _rotAxis | 1;
	_mxRotX = glm::rotate(glm::mat4(1.0f), _rotX, glm::vec3(1.0f, 0.0f, 0.0f));
	_mxRotation = _mxRotX;
	_bRotation = true;
}

void CVillainOne::setRotY(float angle)
{
	_rotY = glm::radians(angle);
	_rotAxis = _rotAxis | 2;
	_mxRotY = glm::rotate(glm::mat4(1.0f), _rotY, glm::vec3(0.0f, 1.0f, 0.0f));
	if (_rotAxis & 1)_mxRotation = _mxRotY * _mxRotX;
	else _mxRotation = _mxRotY;
	_bRotation = true;
}

void CVillainOne::setRotZ(float angle)
{
	_rotZ = glm::radians(angle);
	_mxRotZ = glm::rotate(glm::mat4(1.0f), _rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
	if (_rotAxis == 1) _mxRotation = _mxRotZ * _mxRotX;
	else if (_rotAxis == 2) _mxRotation = _mxRotZ * _mxRotY;
	else if (_rotAxis == 3) _mxRotation = _mxRotZ * _mxRotY * _mxRotX;
	else _mxRotation = _mxRotZ;
	_bRotation = true;
}

void CVillainOne::updateMatrix()
{
	if (_bScale || _bPos || _bRotation)
	{
		_mxTRS = _mxPos * _mxRotation * _mxScale;
		if (_bOnTransform == true) _mxFinal = _mxTransform * _mxTRS;
		else _mxFinal = _mxTRS;
		_bScale = _bPos = _bRotation = false;
	}
	if (_bTransform) {
		_mxFinal = _mxTransform * _mxTRS;
		_bTransform = false;
	}
	// 如多個模型使用相同的 shader program,因每一個模型的 mxTRS 都不同，所以每個frame都要更新
	glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxFinal));

	if (_isflashing) {
		_flashFrameCounter++;
		if (_flashFrameCounter % 40 == 0) {
			if ((_flashFrameCounter / 10) % 20 == 0) setColor(glm::vec3(0.7f));
			else setColor(glm::vec3(0.0f));
		}
	}
	if (_flashFrameCounter >= 80) {
		_isflashing = false;
		setColor(glm::vec3(0.7f));
	}
}

void CVillainOne::setTransformMatrix(glm::mat4 mxMatrix)
{
	_bOnTransform = _bTransform = true;
	_mxTransform = mxMatrix;
}
glm::mat4 CVillainOne::getModelMatrix() { return _mxFinal; }

GLuint CVillainOne::getShaderProgram() { return _shaderProg; }

void CVillainOne::reset()
{
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_color = glm::vec3(1.0f, 1.0f, 1.0f);
	_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	_rotX = 0.0f; _rotY = 0.0f; _rotZ = 0.0f;
	_rotAxis = 0;
	_bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
	_mxScale = glm::mat4(1.0f);
	_mxPos = glm::mat4(1.0f);
	_mxTRS = glm::mat4(1.0f);
	_mxRotX = glm::mat4(1.0f);
	_mxRotY = glm::mat4(1.0f);
	_mxRotZ = glm::mat4(1.0f);
	_mxRotation = glm::mat4(1.0f);
	_mxTransform = glm::mat4(1.0f);
	_mxFinal = glm::mat4(1.0f);
}

bool CVillainOne::checkCollision(glm::vec3 pos, float otherRadius) {
	float dx = pos[0] - _pos.x;
	float dy = pos[1] - _pos.y;
	float combineRadius = circleRadius + otherRadius;
	if ((dx * dx + dy * dy) <= (combineRadius * combineRadius)) return(true);
	else return(false);
}

glm::vec3 CVillainOne::getPos() {
	return(_pos);
}

void CVillainOne::Flashing() {
	setColor(glm::vec3(0.0f));
	_flashFrameCounter = 0;
	_isflashing = true;
}

void CVillainOne::Inter(float dt) {
	glm::vec3 pos = getPos();
	int op = rand() % 3 - 1;
	
	setPos(pos);
}
void CVillainOne::Moving(float dt) {
	glm::vec3 pos = getPos();
	if (pos.y >= 3.0f) {
		if (pos.x > 0) {
			pos.x -= 0.0001f;
		}
		else if (pos.x < 0) {
			pos.x += 0.0001f;
		}
		pos.y -= 0.0002f;
	}
	else {
		for (int i = 0; i < 2; i++) {
			int op = rand() % 3 - 1;
			float speed = ((rand() % (_speed)) / 10.0f) * dt * op;
			pos[i] += speed;
		}
	}
	setPos(pos);
}
