#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#include "CBoss.h"
#include "CShaderPool.h"

CBoss::CBoss() : CVillain(3)
{
	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件
	gun[0].setPos(glm::vec3(_pos.x + 1.37f, _pos.y, 0.0f));
	gun[1].setPos(glm::vec3(_pos.x - 1.37f, _pos.y, 0.0f));
	_blood = 10;
	_draw = false;
}

CBoss::~CBoss()
{
}

void CBoss::draw()
{
	if (_blood > 0) {
		glUseProgram(_shaderProg);
		updateMatrix();
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	}
	else {
		if (bulletthree.getStatus() == 0) {
			bulletthree.draw();
			glUseProgram(_shaderProg);
			updateMatrix();
			glBindVertexArray(_vao);
			glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
			setColor(re_color);
		}
		else dead.draw();
	}

	for (int j = 0; j < _blood && _draw; j++) {
		blood[j].draw();
	}
}

void CBoss::update(float dt)
{
	if (_blood > 0) Moving(dt);
	gun[0].setPos(glm::vec3(_pos.x + 1.37f, _pos.y, 0.0f));
	gun[1].setPos(glm::vec3(_pos.x - 1.37f, _pos.y, 0.0f));
	gun[0].update(dt);
	gun[1].update(dt);

	for (int i = 0; i < _blood; i++) {
		blood[i].setPos(glm::vec3(_pos.x + 0.1f * i, _pos.y + 0.5f, 0.0f));
		blood[i].setPos(glm::vec3(blood[i].getPos().x - 0.25f, blood[i].getPos().y, 0.0f));
	}

	if (getStatus() == 0) {
		if (bulletthree.getStatus() == -1) {
			bulletthree.setPos(_pos);
			bulletthree.setStatus(0);
		}
		else if (bulletthree.getStatus() == 0) {
			bulletthree.update(dt);
			bulletthree.draw();
		}
		else {
			_deadtime += dt;
			dead.setPos(_pos);
			dead.update(dt);
			if (_deadtime >= 1.0f) _blood--;
		}	
	}

	if (_isflashing && _blood > 0) {
		flashTimer += dt;
		if (flashTimer >= nextTime) {
			_flashOn = !_flashOn;
			if (_flashOn) setColor(glm::vec3(0.0f));
			else setColor(glm::vec3(re_color));
			nextTime += 0.1f;
		}
	}
	if (nextTime > 0.5f) {
		_isflashing = false;
		flashTimer = 0.0f;
		nextTime = 0.0f;
		setColor(glm::vec3(re_color));
	}

	if (gun[0].getStatus() == -1 && gun[1].getStatus() == -1) {
		_draw = true;
	}
}

void CBoss::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
}