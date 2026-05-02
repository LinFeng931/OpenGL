// CObj.h
#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "CShape.h"

class CObj : public CShape {
public:
    // resolution：每個 patch 在 u、v 方向的細分段數，預設 10
    CObj();
    virtual ~CObj();

    void Load(const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec2>& uvs,
        const std::vector<glm::vec3>& normals,
        const std::vector<unsigned int>& indices,
        std::vector<GLfloat>& out_extras);

    virtual void draw() override;
    virtual void drawRaw() override;
    virtual void reset() override;
    virtual void update(float dt) override;
    virtual void updateMatrix() override;
    bool Collision(glm::vec3 pos, float otherRadius);
    bool Collision(float x, float z, float otherRadius);
    void setRadius(float radius);

    void DoorAction(float dt);
    void setDoorAction();
    void setPivot(glm::vec3 pos);
    void setDoorRotateType();
    bool getDoorOpen();
    void initBuffer();
    void Break();
    bool isBreak();
    bool breakFinial();
    void setFinial();
    void setTargetAction();
    void setRotate_center(glm::vec3 center, glm::vec3 dir);

    void setKeyActive();
    bool getKeyActive();
    void setDoorActive(bool flag);
    bool getDoorActive();
    void setLockActive(bool flag);
    bool getLockActive();
    //void drawShadow(GLuint shadowShader, const glm::mat4& lightSpaceMatrix);

private:
    void generateTeapot(unsigned int resolution, GLuint pattern);

    // 以下兩個陣列請完整從 freeglut 的 fg_teapot_data.h 複製
    // Patch 索引（32 patches × 4 × 4 控制點）
    static const GLushort teapotPatches[32][4][4];
    // 控制點座標（306 個點 × 3 維）
    static const GLfloat teapotVertices[306][3];

    float _radius;
    bool _breakact;
    float _breakclock;
    bool _isbreak;
    bool _finial;

    bool door = false;
    bool doorActing = false;
    float doorAngle = 0.0f;
    bool doorOpen = false;
    bool doorupdate = false;
    glm::vec3 _pivot; //旋轉軸

    bool re_posflag = false;
    glm::vec3 re_pos;

    bool target = false;
    float target_clock = 0.0f;
    bool _keyActive = false;
    bool _doorActive = true;
    bool _lockActive = true;
};
