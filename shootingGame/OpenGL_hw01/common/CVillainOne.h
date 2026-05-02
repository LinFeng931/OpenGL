#pragma once
#include <glm/glm.hpp>
#include "bullet.h"

#define QUAD_VTX_COUNT  30
#define QUAD_INDEX_COUNT 49
#define QUAD_VTX_ATTR_COUNT 11	// 單位為 float 個數

class CVillainOne
{
public:
	CVillainOne();
	~CVillainOne();
	void setupVertexAttributes();
	GLuint setShader(const char* vshader, const char* fshader);
	void setShaderID(GLuint shaderID);
	void draw();
	void reset();
	void setColor(glm::vec3 vColor); // 設定模型的顏色
	void setScale(glm::vec3 vScale); // 設定模型的縮放比
	void setPos(glm::vec3 vPt); // 設定模型的位置
	void setRotX(float angle); // 設定模型的X軸旋轉角度
	void setRotY(float angle); // 設定模型的Y軸旋轉角度
	void setRotZ(float angle); // 設定模型的Z軸旋轉角度
	void setTransformMatrix(glm::mat4 mxMatrix);
	glm::mat4 getModelMatrix();
	GLuint getShaderProgram();

	bool checkCollision(glm::vec3 pos, float otherRadius);
	glm::vec3 getPos();
	void Flashing();
	void Inter(float dt);
	void Moving(float dt);

protected:
	GLfloat* _points;
	GLuint* _idx;
	GLuint _vao, _vbo, _ebo;
	GLuint _shaderProg, _rotAxis;
	GLint _modelMxLoc;
	bool _bRotation, _bScale, _bPos;
	bool _bTransform, _bOnTransform;
	// _bTransform : true 代表有設定新的轉換矩陣
	// _bOnTransform : true 代表曾經設定過轉換矩陣，用於判斷是否需要更新 model matrix

	glm::vec3 _color;
	glm::vec3 _scale;	// 模型的縮放值
	glm::vec3 _pos;		// 模型的位置
	GLfloat _rotX, _rotY, _rotZ; // 模型的旋轉角度
	glm::mat4 _mxRotX, _mxRotY, _mxRotZ, _mxRotation, _mxScale, _mxPos, _mxTRS;
	glm::mat4 _mxTransform, _mxFinal;
	void updateMatrix();

	//碰撞項數值
	float circleRadius;
	float circleScale;
	bool _isflashing;
	int _flashFrameCounter;
	int _speed;
	/*Bullet* villain_bullet;
	Bullet* pcurrent;
	Bullet* phead;*/
};