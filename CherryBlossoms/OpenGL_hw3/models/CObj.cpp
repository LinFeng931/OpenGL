// CObj.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "CObj.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// --- 3. 建構／解構子，以及 Draw 系列函式 ---
CObj::CObj() : CShape() {
    _radius = 0.0f;
    _pivot = glm::vec3(-0.5f, 0.0f, 0.0f);
    _breakact = false;
    _breakclock = 0.0f;
    _isbreak = false;
    _finial = false;
}

CObj::~CObj() {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
    if (_points) delete[] _points;
    if (_idx)    delete[] _idx;
}

void CObj::Load(const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec2>& uvs,
    const std::vector<glm::vec3>& normals,
    const std::vector<unsigned int>& indices,
    std::vector<GLfloat>& out_extras)
{
    _vtxCount = positions.size();
    _idxCount = indices.size();
    _vtxAttrCount = 19;

    // 配置 _points：每個頂點有 11 個 float
    if (_points) delete[] _points;
    _points = new float[_vtxCount * _vtxAttrCount];

    for (size_t i = 0; i < _vtxCount; ++i) {
        size_t base = i * _vtxAttrCount;

        // Position
        _points[base + 0] = positions[i].x;
        _points[base + 1] = positions[i].y;
        _points[base + 2] = positions[i].z;

        // Color（可以先填預設值，之後用 setColor() 改）
        _points[base + 3] = 1.0f;
        _points[base + 4] = 1.0f;
        _points[base + 5] = 1.0f;

        // Normal
        if (i < normals.size()) {
            _points[base + 6] = normals[i].x;
            _points[base + 7] = normals[i].y;
            _points[base + 8] = normals[i].z;
        }
        else {
            _points[base + 6] = 0.0f;
            _points[base + 7] = 0.0f;
            _points[base + 8] = 1.0f;
        }

        // UV
        if (i < uvs.size()) {
            _points[base + 9] = uvs[i].x;
            _points[base + 10] = uvs[i].y;
        }
        else {
            _points[base + 9] = 0.0f;
            _points[base + 10] = 0.0f;
        }
    }

    // 複製 indices 到 _idx
    if (_idx) delete[] _idx;
    _idx = new GLuint[_idxCount];
    for (size_t i = 0; i < _idxCount; ++i)
        _idx[i] = indices[i];

    initBuffer();
}


void CObj::draw() {
    glUseProgram(_shaderProg);
    updateMatrix();
    glBindVertexArray(_vao);
    glUniform1i(_shadingModeLoc, _uShadingMode);
    if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
    glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CObj::drawRaw() {
    uploadTextureFlags();
    uploadMaterial();
    updateMatrix();
    glBindVertexArray(_vao);
    glUniform1i(_shadingModeLoc, _uShadingMode);
    if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
    glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CObj::reset() {
    CShape::reset();
}

void CObj::update(float dt) {
    // 可做動畫或自轉
    if (doorupdate) {
        DoorAction(dt);
    }

    if (_breakact) {
        _breakclock += dt;
        if (_breakclock > 1.5f) {
            setColor(glm::vec4(0.0f));
            _breakact = false;
            _isbreak = true;
        }    
    }

    if (target) {
        target_clock += dt;
        float floatAmplitude = 0.006f;      // 漂浮最大上下幅度（正負1）
        float floatSpeed = 2.0f;          // 控制上下一次週期所需時間（越大越慢）

        float offsetY = sin(target_clock * floatSpeed) * floatAmplitude;

        setPos(glm::vec3(_pos.x, _pos.y + offsetY, _pos.z));
    }
}

bool CObj::Collision(glm::vec3 pos, float otherRadius) {
    if (_isbreak || _breakact) return(false);
    float dx = pos[0] - _pos.x;
    float dy = pos[1] - _pos.y;
    float dz = pos[2] - _pos.z;

    float combineRadius = _radius + otherRadius;
    if ((dx * dx + dy * dy + dz * dz) <= (combineRadius * combineRadius)) {
        return(true);
    }
    else return(false);
}

bool CObj::Collision(float x, float z, float otherRadius) { //水平檢測
    if (_isbreak || _breakact) return(false);
    float dx = x - _pos.x;
    float dz = z - _pos.z;

    float combineRadius = _radius + otherRadius;
    if ((dx * dx + dz * dz) <= (combineRadius * combineRadius)) {
        return(true);
    }
    else return(false);
}

void CObj::setRadius(float radius) {
    _radius = radius;
}

void CObj::setDoorAction() {
    doorOpen = !doorOpen;
    doorupdate = true;
    doorActing = true;
}
void CObj::DoorAction(float dt) {

    if (doorActing && !re_posflag) {
        re_pos = _pos;          // 記錄原始門的位置
        re_posflag = true;
    }
    if (doorOpen && doorAngle < 90.0f) {
        doorAngle += 40.0f * dt;
        if (doorAngle > 90.0f) {
            doorAngle = 90.0f;
            doorActing = false;
            doorupdate = false;
        }
    }
    else if (!doorOpen && doorAngle > 0.0f) {
        doorAngle -= 40.0f * dt;
        if (doorAngle < 0.0f) {
            doorAngle = 0.0f;
            doorActing = false;
            doorupdate = false;
        }
    }

    if (!re_posflag) return; // 避免還沒開始時就跑下面程式

    glm::vec3 pivotWorld = re_pos + _pivot;

    // 建立繞 Y 軸旋轉矩陣（以原點為中心）
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(doorAngle), glm::vec3(0, 1, 0));

    // 將 -_pivot 向量旋轉（取得模型應該移動多少）
    glm::vec3 offset = glm::vec3(rotMat * glm::vec4(-_pivot, 0.0f));

    // 最終位置 = pivotWorld + 旋轉後的 offset
    glm::vec3 finalPos = pivotWorld + offset;

    // 設定旋轉與位置
    setRotate(doorAngle, glm::vec3(0, 1, 0));
    setPos(finalPos);
}

void CObj::setPivot(glm::vec3 pos) {
    _pivot = pos;
}

void CObj::updateMatrix()
{
    if (_bScale || _bPos || _bRotation)
    {
        // 1. 建立基本變換矩陣
        _mxScale = glm::scale(glm::mat4(1.0f), _scale);
        _mxTrans = glm::translate(glm::mat4(1.0f), _pos);

        // 2. 門的旋轉要繞 pivot 做：T(pivot) * R(angle) * T(-pivot)
        glm::mat4 T_pivot = glm::translate(glm::mat4(1.0f), _pivot);
        glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(_rotateAngle), _rotateAxis);
        glm::mat4 T_negPivot = glm::translate(glm::mat4(1.0f), -_pivot);
        _mxRotation = T_pivot * R * T_negPivot;

        // 3. 組合
        _mxTRS = _mxTrans * _mxRotation * _mxScale;

        // 4. 如果有額外變換
        if (_bOnTransform)
            _mxFinal = _mxTransform * _mxTRS;
        else
            _mxFinal = _mxTRS;

        _bScale = _bPos = _bRotation = false;
    }

    if (_bTransform) {
        _mxFinal = _mxTransform * _mxTRS;
        _bTransform = false;
    }

    // 更新 shader 的 uniform
    glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxFinal));
}

bool CObj::getDoorOpen() {
    return(doorOpen);
}

void CObj::initBuffer() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vtxCount * _vtxAttrCount, _points, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _idxCount, _idx, GL_STATIC_DRAW);

    // 設定 attribute pointers (位置3 + 顏色3 + 法線3 + UV2 = 11 floats)
    GLsizei stride = sizeof(float) * _vtxAttrCount;

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    // UV
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 9));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void CObj::Break() {
    _bObjColor = true;
    setColor(glm::vec4(1.0f));
    //_hasTexture = false;
    
    _breakact = true;
}

bool CObj::isBreak() {
    return(_isbreak);
}

bool CObj::breakFinial() {
    return(_finial);
}

void CObj::setFinial() {
    _finial = true;
}

void CObj::setTargetAction() {
    target = true;
}

void CObj::setRotate_center(glm::vec3 center, glm::vec3 direction)
{
    // 只取水平方向
    glm::vec2 dir2D = glm::normalize(glm::vec2(direction.x, direction.z));

    // 基準方向：面對 -Z
    glm::vec2 forward = glm::vec2(0.0f, -1.0f);

    // atan2：不會跳角，輸出為 [-π, π]
    float angleRad = atan2(dir2D.x * forward.y - dir2D.y * forward.x,
        glm::dot(dir2D, forward));

    float angleDeg = glm::degrees(angleRad);  // 轉為角度

    setRotate(angleDeg, glm::vec3(0, 1, 0));  // 繞 Y 軸轉
}

void CObj::setKeyActive() {
    _keyActive = true;
}
bool CObj::getKeyActive() {
    return(_keyActive);
}

void CObj::setDoorActive(bool flag) {
    _doorActive = flag;
}
bool CObj::getDoorActive() {
    return(_doorActive);
}

void CObj::setLockActive(bool flag) {
    _lockActive = flag;
}
bool CObj::getLockActive() {
    return(_lockActive);
}