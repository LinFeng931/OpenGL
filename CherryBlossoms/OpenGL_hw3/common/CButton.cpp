// CButton.cpp
#include "CButton.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CButton::CButton(float width, float height)
{
    _type = 0;
    _colorOn = glm::vec4(1.0f,1.0f,1.0f,1.0f); _colorOff = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	_width = width, _height = height;  // 按鈕寬度、高度
	_bActive = false;  // 按鈕初始狀態為未按下
    _vtxAttrCount = 8;  _vtxCount = 28; _idxCount = 42;
    _points = new GLfloat[_vtxCount * _vtxAttrCount];
    _idx = new GLuint[_idxCount];
    float hw = _width * 0.5f, hh = _height * 0.5f;  
    float hw2 = _width * 0.1f, hh2 = _height * 0.5f;
    GLfloat verts[28][8] = { // 以中心為原點的四頂點 ： x, y, z, r, g, b(dummy), u,v
        {-hw, -hh, 0.0f,   0,0,0,   0,0}, 
        { hw, -hh, 0.0f,   0,0,0,   1,0},
        { hw,  hh, 0.0f,   0,0,0,   1,1},
        {-hw,  hh, 0.0f,   0,0,0,   0,1},

        //鑰匙
        { 3.0f,  5.0f, 0.0f,   0,0,0,   0,0},
        { 12.0f,  5.0f, 0.0f,   0,0,0,   1,0},
        { 12.0f,  11.0f, 0.0f,   0,0,0,   1,1},
        { 3.0f,  11.0f, 0.0f,   0,0,0,   0,1},

        { -3.0f,   -12.0f,  0.0f,   0,0,0,   0,0 },
        {  3.0f,   -12.0f,  0.0f,   0,0,0,   1,0 },
        {  3.0f,  16.0f,  0.0f,   0,0,0,   1,1 },
        { -3.0f,  16.0f,  0.0f,   0,0,0,   0,1 },

        { -9.0f,  -18.0f,  0.0f,   0,0,0,   0,0 },
        {  9.0f,  -18.0f,  0.0f,   0,0,0,   1,0 },
        {  9.0f,   -12.0f,  0.0f,   0,0,0,   1,1 },
        { -9.0f,   -12.0f,  0.0f,   0,0,0,   0,1 },

        { -9.0f, -30.0f,  0.0f,   0,0,0,   0,0 },
        {  9.0f, -30.0f,  0.0f,   0,0,0,   1,0 },
        {  9.0f, -24.0f,  0.0f,   0,0,0,   1,1 },
        { -9.0f, -24.0f,  0.0f,   0,0,0,   0,1 },

        { -9.0f, -24.0f,  0.0f,   0,0,0,   0,0 },
        { -3.0f,  -24.0f,  0.0f,   0,0,0,   1,0 },
        { -3.0f,   -18.0f,  0.0f,   0,0,0,   1,1 },
        { -9.0f,  -18.0f,  0.0f,   0,0,0,   0,1 },

        {  3.0f, -24.0f,  0.0f,   0,0,0,   0,0 },
        { 9.0f, -24.0f,  0.0f,   0,0,0,   1,0 },
        { 9.0f,  -18.0f,  0.0f,   0,0,0,   1,1 },
        {  3.0f,  -18.0f,  0.0f,   0,0,0,   0,1 },
    };
    for (int i = 0; i < 28; ++i)
        for (int j = 0; j < _vtxAttrCount; ++j)
            _points[i * _vtxAttrCount + j] = verts[i][j];
    GLuint idxs[42] = { 
        0,1,2,  2,3,0,

        4,5,6,  6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20,
        24,25,26, 26,27,24
    };
    for (int i = 0; i < 42; ++i) _idx[i] = idxs[i];
    // CSprite2D 預設 uniform color
}

CButton::~CButton() {}

void CButton::init(GLuint shaderProg) {  // 必須放到 loadscene 呼叫
    setShaderID(shaderProg);
    setupVertexAttributes();  // CSprite2D 提供
    setColor(_colorOff);  // uniform color
}

void CButton::setScreenPos(float sx, float sy) {
    setPos(glm::vec3(sx, sy, 0.0f));
    // 更新滑鼠偵測範圍
    _minXY = glm::vec2(sx - _width * 0.5f, sy - _height * 0.5f);
    _maxXY = glm::vec2(sx + _width * 0.5f, sy + _height * 0.5f);
}

bool CButton::handleClick(float mx, float my) {
    if (!isMouseOver(mx, my)) return false; // 先判斷滑鼠位置是否在按鈕上
    _bActive = !_bActive; // 切換是否被按下的狀態
    setColor(_bActive ? _colorOn : _colorOff);  // 設定按鈕顏色
    return true;
}

bool CButton::isMouseOver(float mx, float my) const {
    return mx >= _minXY.x && mx <= _maxXY.x
        && my >= _minXY.y && my <= _maxXY.y;
}

void CButton::draw()
{
    glUseProgram(_shaderProg);
    glBindVertexArray(_vao);
    updateMatrix();
    glUniform1i(_coloringModeLoc, _coloringMode);
    if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));

    if(_type == 0) glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    else if(_type == 1) glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));

    glBindVertexArray(0);
}

void CButton::drawRaw()
{
    glBindVertexArray(_vao);
    updateMatrix();
    glUniform1i(_coloringModeLoc, _coloringMode);
    if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
    glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CButton::setType(int type) {
    _type = type;
    if (type == 1) {
        _colorOn = glm::vec4(1.0f, 0.84f, 0.0f, 1.0f);
        _colorOff = glm::vec4(1.0f, 0.84f, 0.0f, 1.0f);
    }
}