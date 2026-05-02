#pragma once
#include <glm/glm.hpp>


class CShape
{
public:
	CShape();
	virtual ~CShape();
	virtual void draw();
	virtual void reset();
	virtual void update(float dt);
	void setupVertexAttributes();
	void setShaderID(GLuint shaderID);
	void setColor(glm::vec3 vColor); // 設定模型的顏色
	void setScale(glm::vec3 vScale); // 設定模型的縮放比
	void setPos(glm::vec3 vPt); // 設定模型的位置
	void setRotX(float angle); // 設定模型的X軸旋轉角度
	void setRotY(float angle); // 設定模型的Y軸旋轉角度
	void setRotZ(float angle); // 設定模型的Z軸旋轉角度
	void setTransformMatrix(glm::mat4 mxMatrix);
	void updateMatrix();

	glm::mat4 getModelMatrix();
	GLuint getShaderProgram();

protected:
	int _vtxCount, _vtxAttrCount, _idxCount; // 頂點數、頂點屬性數、點頂索引值數
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
	glm::vec3 m_color;
};