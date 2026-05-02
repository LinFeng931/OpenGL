#pragma once
#include <glfw/include/GLFW/glfw3.h>

bool loadOBJ(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals,
    std::vector<unsigned int>& out_indices,
    std::vector<GLfloat> &extras);