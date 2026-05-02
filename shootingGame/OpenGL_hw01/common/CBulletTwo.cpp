#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CBulletTwo.h"

CBulletTwo::CBulletTwo() : CShape()
{
    _vtxCount = 13;
    _vtxAttrCount = 11;
    _idxCount = 36;

    _points = new GLfloat[_vtxCount * _vtxAttrCount]{
        // 中心點
         0.0f,  0.0f,   0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.5f,

         // 外圍點（每 30 度一個，共 12 個）
          0.15f,   0.0f,    0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.5f,
          0.1299f, 0.075f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.933f, 0.75f,
          0.075f,  0.1299f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.75f, 0.933f,
          0.0f,    0.15f,   0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f,
         -0.075f,  0.1299f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.25f, 0.933f,
         -0.1299f, 0.075f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.067f, 0.75f,
         -0.15f,   0.0f,    0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.5f,
         -0.1299f,-0.075f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.067f, 0.25f,
         -0.075f, -0.1299f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.25f, 0.067f,
          0.0f,   -0.15f,   0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.0f,
          0.075f, -0.1299f, 0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.75f, 0.067f,
          0.1299f,-0.075f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.933f, 0.25f
    };

    _idx = new GLuint[_idxCount]{
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7,
        0, 7, 8,
        0, 8, 9,
        0, 9, 10,
        0,10, 11,
        0,11, 12,
        0,12, 1
    };

	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件

	g_angle = 0.0f;
    _path = 1;
    _speed = 4.0f;
    circleRadius = 0.15f;
}

CBulletTwo::~CBulletTwo()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	glDeleteProgram(_shaderProg);  //釋放 shader program
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CBulletTwo::draw()
{
	glUseProgram(_shaderProg);
	updateMatrix();
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
}

void CBulletTwo::update(float dt)
{
	g_angle += 180.0f * dt;
	if (g_angle > 360.0f) g_angle -= 360.0f;
	setRotZ(g_angle);

    switch (_path)
    {
    case 1:
        setPos(glm::vec3(_pos.x - 0.001f, _pos.y - _speed*dt, 0.0f));
        break;
    case 2:
        setPos(glm::vec3(_pos.x, _pos.y - _speed*dt, 0.0f));
        break;
    case 3:
        setPos(glm::vec3(_pos.x + 0.001f, _pos.y - _speed*dt, 0.0f));
        break;
    default:
        break;
    } 
}

void CBulletTwo::setPath(int path) {
    _path = path;
}

void CBulletTwo::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}

glm::vec3 CBulletTwo::getPos() {
    return(_pos);
}

float CBulletTwo::getRadius() {
    return(circleRadius);
}