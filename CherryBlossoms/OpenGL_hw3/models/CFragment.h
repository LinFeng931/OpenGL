#pragma once
#include "CObj.h"

class CFragment : public CObj {
public:
    CFragment(glm::vec3 pos, glm::vec3 velocity);
    void update(float dt) override;

private:
    glm::vec3 _velocity;
    float _lifetime = 0.0f;
    const float _maxLife = 2.0f;
};