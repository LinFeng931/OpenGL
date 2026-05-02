#pragma once
#include <glm/glm.hpp>
#include "CVillain.h"
#include "CVillainTwo.h"

void Villain();
void v_draw();
void v_update(float dt);
int v_getAmount();
int v_getIndex();
CVillain& v_get(int t);
void v_release();
bool v_OK(int t);
