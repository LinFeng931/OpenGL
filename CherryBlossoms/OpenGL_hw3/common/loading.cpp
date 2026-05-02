#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include <vector>

//#include "../stb_image.h"
#include <glew/include/GL/glew.h>
#include <glfw/include/GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../common/initshader.h"
#include "../common/arcball.h"
#include "../common/wmhandler.h"

bool loadOBJ(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals,
    std::vector<unsigned int>& out_indices,
    std::vector<GLfloat>& out_extras // 新增的輸出，依序為 Lightmap UV、Tangent、Bitangent
) {
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::vector<glm::vec3> final_vertices;
    std::vector<glm::vec2> final_uvs;
    std::vector<glm::vec3> final_normals;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string prefix;
        s >> prefix;

        if (prefix == "v") {
            glm::vec3 v; s >> v.x >> v.y >> v.z;
            temp_vertices.push_back(v);
        }
        else if (prefix == "vt") {
            glm::vec2 uv; s >> uv.x >> uv.y;
            uv.y = 1.0f - uv.y; // Flip V for OpenGL
            temp_uvs.push_back(uv);
        }
        else if (prefix == "vn") {
            glm::vec3 n; s >> n.x >> n.y >> n.z;
            temp_normals.push_back(n);
        }
        else if (prefix == "f") {
            std::string vertexStr;
            for (int i = 0; i < 3; ++i) {
                s >> vertexStr;
                std::istringstream vs(vertexStr);
                std::string vIdxStr, vtIdxStr, vnIdxStr;

                std::getline(vs, vIdxStr, '/');
                std::getline(vs, vtIdxStr, '/');
                std::getline(vs, vnIdxStr, '/');

                unsigned int vIdx = std::stoi(vIdxStr);
                unsigned int vtIdx = std::stoi(vtIdxStr);
                unsigned int vnIdx = std::stoi(vnIdxStr);

                final_vertices.push_back(temp_vertices[vIdx - 1]);
                final_uvs.push_back(temp_uvs[vtIdx - 1]);
                final_normals.push_back(temp_normals[vnIdx - 1]);

                out_indices.push_back(static_cast<unsigned int>(final_vertices.size()) - 1);

                // 額外補上 Lightmap UV + Tangent + Bitangent（共 8 個 float）
                // 預設為 0
                out_extras.push_back(0.0f); // lightmap UV x
                out_extras.push_back(0.0f); // lightmap UV y

                out_extras.push_back(0.0f); // tangent x
                out_extras.push_back(0.0f); // tangent y
                out_extras.push_back(0.0f); // tangent z

                out_extras.push_back(0.0f); // bitangent x
                out_extras.push_back(0.0f); // bitangent y
                out_extras.push_back(0.0f); // bitangent z
            }
        }
    }

    out_vertices = std::move(final_vertices);
    out_uvs = std::move(final_uvs);
    out_normals = std::move(final_normals);

    return true;
}
