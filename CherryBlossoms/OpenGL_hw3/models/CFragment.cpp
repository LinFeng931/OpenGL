#include "CFragment.h"

CFragment::CFragment(glm::vec3 pos, glm::vec3 velocity)
    : _velocity(velocity) {
    setPos(pos);
    setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // 白色
    setScale(glm::vec3(0.1f)); // 小碎片
}

void CFragment::update(float dt) {
    _lifetime += dt;

    // 簡單的移動邏輯
    glm::vec3 pos = getPos();
    pos += _velocity * dt;
    setPos(pos);

    // 也可以加上漸變透明
    float alpha = 1.0f - (_lifetime / _maxLife);
    setColor(glm::vec4(1.0f, 1.0f, 1.0f, alpha));
}
