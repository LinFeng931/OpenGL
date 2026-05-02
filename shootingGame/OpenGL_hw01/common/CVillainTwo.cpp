#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#include "CVillainTwo.h"
#include "CShaderPool.h"

CVillainTwo::CVillainTwo() : CVillain(2)
{
	
	g_angle = 0.0f;

	//	setupVertexAttributes(); // 這行改在主程式由物件呼叫，就能讓類別可宣告實體物件
}

CVillainTwo::~CVillainTwo()
{
}

void CVillainTwo::update(float dt)
{
	g_angle += 180.0f * dt;
	if (g_angle > 360.0f) g_angle -= 360.0f;
	setRotZ(g_angle);

	if (_blood > 0) Moving(dt);

	for (int i = 0; i < _blood; i++) {
		blood[i].setPos(glm::vec3(_pos.x + 0.1f * i, _pos.y + 0.5f, 0.0f));
		blood[i].setPos(glm::vec3(blood[i].getPos().x - 0.25f, blood[i].getPos().y, 0.0f));
	}

	if (getStatus() == 0) {
		_deadtime += dt;
		dead.setPos(_pos);
		dead.update(dt);
		if (_deadtime >= 1.0f) _blood--;
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

	if (_isflashing != true) {
		if (_color != re_color) re_color = _color;
	}
}

void CVillainTwo::reset() {
	CShape::reset();
	// 如有需要，可加入其他特定於四邊形的重設動作
	_blood = 6;
	flashTimer = 0.0f;
	nextTime = 0.0f;
	_flashOn = false;
	_deadtime = 0.0f;

	PosX = true;
	PosY = true;
	startPos = true;
	moving_timer = 0.0f;
	dead.reset();

	glm::vec3 newColor = glm::vec3(
		rand() % 10 * 0.1f + 0.1f,
		rand() % 10 * 0.1f + 0.1f,
		rand() % 10 * 0.1f + 0.1f
	);
	setColor(newColor);
	re_color = newColor;
}