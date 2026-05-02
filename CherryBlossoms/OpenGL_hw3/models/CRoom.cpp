#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CRoom.h"

CRoom::CRoom(float wallx1, float wallx2, float wallz1, float wallz2) : CShape()
{
	_wall_x1 = wallx1; _wall_x2 = wallx2; _wall_z1 = wallz1; _wall_z2 = wallz2;
	wall[0].setRotate(90.0f, glm::vec3(0, 1, 0));
	wall[0].setScale(glm::vec3(18.0f, 18.0f, 1.0f));
	wall[0].setPos(glm::vec3(_wall_x1, 5.0f, (_wall_z1+_wall_z2)/2.0f));

	wall[1].setRotate(-90.0f, glm::vec3(0, 1, 0));
	wall[1].setScale(glm::vec3(18.0f, 18.0f, 1.0f));
	wall[1].setPos(glm::vec3(_wall_x2, 5.0f, (_wall_z1 + _wall_z2) / 2.0f));

	wall[2].setScale(glm::vec3(18.0f, 18.0f, 1.0f));
	wall[2].setPos(glm::vec3((_wall_x1 + _wall_x2) / 2.0f, 5.0f, _wall_z1));

	wall[3].setRotate(180.0f, glm::vec3(0, 1, 0));
	wall[3].setScale(glm::vec3(18.0f, 18.0f, 1.0f));
	wall[3].setPos(glm::vec3((_wall_x1 + _wall_x2) / 2.0f, 5.0f, _wall_z2));

	top.setScale(glm::vec3(18.0f, 18.0f, 1.0f));
	top.setRotate(-90.0f, glm::vec3(1, 0, 0));
	top.setPos(glm::vec3((_wall_x1 + _wall_x2)/2.0f, 12.0f, (_wall_z1 + _wall_z2) / 2.0f));

	ground.setScale(glm::vec3(18.0f, 18.0f, 1.0f));
	ground.setRotate(90.0f, glm::vec3(1, 0, 0));
	ground.setPos(glm::vec3((_wall_x1 + _wall_x2) / 2.0f, 0.0f, (_wall_z1 + _wall_z2) / 2.0f));

	for (int i = 0; i < 4; i++) {
		pillair[i].setScale(glm::vec3(0.5f, 2.0f, 0.5f));
	}
	pillair[0].setPos(glm::vec3(_wall_x1 - 0.3f, 6.0f, _wall_z1 - 0.3f));
	pillair[1].setPos(glm::vec3(_wall_x1 - 0.3f, 6.0f, _wall_z2 + 0.3f));
	pillair[2].setPos(glm::vec3(_wall_x2 + 0.3f, 6.0f, _wall_z2 + 0.3f));
	pillair[3].setPos(glm::vec3(_wall_x2 + 0.3f, 6.0f, _wall_z1 - 0.3f));

	for (int i = 4; i < 8; i++) {
		pillair[i].setScale(glm::vec3(0.5f, 3.5f, 0.5f));
	}
	pillair[4].setRotate(90.0f, glm::vec3(0, 0, 1));
	pillair[4].setPos(glm::vec3((_wall_x1 + _wall_x2) / 2.0f, 11.7f, _wall_z1 - 0.3f));
	pillair[5].setRotate(90.0f, glm::vec3(0, 0, 1));
	pillair[5].setPos(glm::vec3((_wall_x1 + _wall_x2) / 2.0f, 11.7f, _wall_z2 + 0.3f));
	pillair[6].setRotate(90.0f, glm::vec3(1, 0, 0));
	pillair[6].setPos(glm::vec3(_wall_x1 - 0.3f, 11.7f, (_wall_z1 + _wall_z2) / 2.0f));
	pillair[7].setRotate(90.0f, glm::vec3(1, 0, 0));
	pillair[7].setPos(glm::vec3(_wall_x2 + 0.3f, 11.7f, (_wall_z1 + _wall_z2) / 2.0f));
}

CRoom::~CRoom()
{
	glDeleteBuffers(1, &_vbo);  //先釋放 VBO 與 EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //再釋放 VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CRoom::draw()
{
	for (int i = 0; i < 4; i++) {
		wall[i].uploadMaterial();
		wall[i].draw();
	}
}

void CRoom::drawRaw()
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

void CRoom::update(float dt)
{

}

void CRoom::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}

int CRoom::Collisionx(float posx, float posz, bool doorOpen, float doorposx, float doorposz, float distance) {
	//wall[0]
	if (wall[0].getType() != 1) {
		if ((_wall_x1 - posx)* (_wall_x1 - posx) < distance) {
			hr1 = true;
			return(1);
		}
	}
	else {
		if (doorOpen) {
			if ((posz - doorposz) * (posz - doorposz) < 4.0f) return(0);
			else if ((_wall_x1 - posx) * (_wall_x1 - posx) < distance) {
				hr1 = true;
				return(1);
			}
		}
		else if ((_wall_x1 - posx) * (_wall_x1 - posx) < distance) {
			hr1 = true;
			return(1);
		}
	}

	//wall[1]
	if (wall[1].getType() != 1) {
		if ((_wall_x2 - posx)* (_wall_x2 - posx) < distance) {
			hr2 = true;
			return(1);
		}
	}
	else {
		if (doorOpen) {
			if ((posz - doorposz)* (posz - doorposz) < 4.0f) return(0);
			else if ((_wall_x2 - posx) * (_wall_x2 - posx) < distance) {
				hr2 = true;
				return(1);
			}
		}
		else if ((_wall_x2 - posx) * (_wall_x2 - posx) < distance) {
			hr2 = true;
			return(1);
		}
	}

	return(0);
}

int CRoom::Collisionz(float posx, float posz, bool doorOpen, float doorposx, float doorposz, float distance) {
	//wall[2]
	if (wall[2].getType() != 1) {
		if ((_wall_z1 - posz) * (_wall_z1 - posz) < distance) {
			di1 = true;
			return(1);
		}
	}
	else {
		if (doorOpen) {
			if ((posx - doorposx) * (posx - doorposx) < 4.0f) return(0);
			else if ((_wall_z1 - posz) * (_wall_z1 - posz) < distance) {
				di1 = true;
				return(1);
			}
		}
		else if ((_wall_z1 - posz) * (_wall_z1 - posz) < distance) {
			di1 = true;
			return(1);
		}
	}

	//wall[3]
	if (wall[3].getType() != 1) {
		if ((_wall_z2 - posz) * (_wall_z2 - posz) < distance) {
			di2 = true;
			return(1);
		}
	}
	else {
		if (doorOpen) {
			if ((posx - doorposx) * (posx - doorposx) < 4.0f) return(0);
			else if ((_wall_z2 - posz) * (_wall_z2 - posz) < distance) {
				di2 = true;
				return(1);
			}
		}
		else if ((_wall_z2 - posz) * (_wall_z2 - posz) < distance) {
			di2 = true;
			return(1);
		}
	}

	return(0);
}

int CRoom::getCollisionWall() {
	if (hr1)return(-1);
	if (hr2)return(1);
	if (di1)return(1);
	if (di2)return(-1);
}

void CRoom::resetCollision() {
	hr1 = false;
	hr2 = false;
	di1 = false;
	di2 = false;
}