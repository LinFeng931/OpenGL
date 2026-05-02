#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CMap.h"

CMap::CMap() : CShape()
{
	
}

CMap::~CMap()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CMap::draw()
{
	
}

void CMap::drawRaw()
{
	for (int i = 0; i < 4; i++) {
		wall[i].uploadMaterial();
		wall[i].drawRaw();
	}
	top.uploadMaterial();
	top.drawRaw();
	ground.uploadMaterial();
	ground.drawRaw();
}

void CMap::update(float dt)
{

}

void CMap::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}