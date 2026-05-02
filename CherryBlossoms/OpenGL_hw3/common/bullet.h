#pragma once
#include <glew/include/GL/glew.h>
#include <glfw/include/GLFW/glfw3.h>
#include "../models/CBullet.h"

void shoot(glm::vec3 pos, glm::vec3 front);
void bullet_update(float dt);
void bullet_draw(glm::mat4 mxView, glm::mat4 mxProj);
void bullet_load();