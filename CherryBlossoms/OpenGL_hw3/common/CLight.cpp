// CLight.cpp

#include "CLight.h"
#include <glm/gtc/type_ptr.hpp>
#include "typedefs.h"
#include <glm/gtc/random.hpp>

CLight::CLight(
    glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
    float constant, float linear, float quadratic) {
    _position = position;
    _ambient = ambient;  _diffuse = diffuse;  _specular = specular;
    _ambient.w = 1.0f; _diffuse.w = 1.0f; _specular.w = 1.0f;
    _constant = constant; _linear = linear; _quadratic = quadratic;
    _intensity = 1.0f;
    _type = LightType::POINT; _posStart = position;
    _direction = glm::vec3(1.0f); _target = glm::vec3(1.0f);
    _innerCutOff = 0.0f; _outerCutOff = 0.0f; _exponent = 1.0f;
    // 代表 light 的模型，可自行更換
    _displayOn = true; _motionOn = false; _clock = 0.0f;
    _lighingOn = true; _lightObj.setPos(position);

    _activeType = 0;
    _clock2 = 0.0f;
    _flashingflag = 0;
    _lighingOnflag = false;
    _wingsRotate = 0.0f;
    _wingsRotateFlag = false;
    _wingTimer = 0.0f;
    _targetCenter = glm::vec3(0.0f);
}
// 方向式聚光燈建構元
//CLight::CLight(glm::vec3 position, glm::vec3 direction, float innerCutOffDeg, float outerCutOffDeg,
//    glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) {
//    _position = position; _direction = direction; _enabled = true;
//    _innerCutOff = innerCutOffDeg; _outerCutOff = outerCutOffDeg;
//    _ambient = ambient;   _diffuse = diffuse; _specular = specular;
//    _constant = constant; _linear = linear;   _quadratic = quadratic;
//}
// 目標式聚光燈建構元
CLight::CLight(glm::vec3 position, glm::vec3 target, float innerCutOffDeg, float outerCutOffDeg, float exponent,
    glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float constant, float linear, float quadratic) {
    _position = position; _target = target; _direction = glm::normalize(target - position);
    _intensity = 1.0f;  _exponent = exponent;
    _innerCutOff = glm::cos(glm::radians(innerCutOffDeg));
    _outerCutOff = glm::cos(glm::radians(outerCutOffDeg));
    _ambient = ambient;  _diffuse = diffuse;  _specular = specular;
    _ambient.w = 1.0f; _diffuse.w = 1.0f; _specular.w = 1.0f;
    _constant = constant; _linear = linear; _quadratic = quadratic;
    _type = LightType::SPOT; _posStart = position;
    _displayOn = true; _motionOn = false; _clock = 0.0f;
    _lighingOn = true; _lightObj.setPos(position);
}

CLight::~CLight() = default; // 讓編譯器產生預設的解構元

// 初始位置由建構元來設定，更新光源位置則是呼叫 setPos
void CLight::setPos(glm::vec3 pos) {
    _position = pos;
    flag_position = true;
    // 如果是 spot light 則更新 direction
    if (_type == LightType::SPOT) {
        _direction = glm::normalize(_target - _position);
    }
}
glm::vec3 CLight::getPos() { return _position; }

void CLight::setAmbient(glm::vec4 amb) { flag_ambient = true; _ambient = amb; _needsUpdate = true; }
glm::vec4 CLight::getAmbient() { return _ambient; }

void CLight::setDiffuse(glm::vec4 diff) { flag_diffuse = true; _diffuse = diff; _needsUpdate = true; }
glm::vec4 CLight::getDiffuse() { return _diffuse; }

void CLight::setSpecular(glm::vec4 spec) { flag_specular = true; _specular = spec; _needsUpdate = true; }
glm::vec4 CLight::getSpecular() { return _specular; }

void CLight::setIntensity(float intensity) {
    _intensity = intensity;
    flag_intensity = true;
    _ambient = _ambient * _intensity;
    _diffuse = _diffuse * _intensity;
    _specular = _specular * _intensity;
    _ambient.w = 1.0f; _diffuse.w = 1.0f; _specular.w = 1.0f;
}

void CLight::setAttenuation(float c, float l, float q) {
    _constant = c; _linear = l;  _quadratic = q;
    flag_attenuation = true;
    _needsUpdate = true;
}
void CLight::getAttenuation(float& c, float& l, float& q) {
    c = _constant; l = _linear;  q = _quadratic;
}

void CLight::setLightOn(bool enable) { _lighingOn = enable; _lighingOnflag = true; _needsUpdate = true; }
bool CLight::isLightOn() { return _lighingOn; }

void CLight::setMotionEnabled() { _motionOn = !_motionOn; _needsUpdate = true; }

// 設定 shaderID 時，預設光源物件顯示為 true
void CLight::setShaderID(GLuint shaderProg, std::string name, bool displayon)
{
    _shaderID = shaderProg; _lightname = name;
    // 如果沒有啟用，就把強度設為零
    glm::vec4 amb = _lighingOn ? _ambient : glm::vec4(0.0f);
    glm::vec4 diff = _lighingOn ? _diffuse : glm::vec4(0.0f);
    glm::vec4 spec = _lighingOn ? _specular : glm::vec4(0.0f);

    GLint loc;
    //可以加強這邊的效能(+flag) 或移至其他函式
    if (flag_position) {
        loc = glGetUniformLocation(_shaderID, (_lightname + ".position").c_str());
        glUniform3fv(loc, 1, glm::value_ptr(_position));
        flag_position = false;
        _needsUpdate = true;
    }

    if (flag_ambient) {
        loc = glGetUniformLocation(_shaderID, (_lightname + ".ambient").c_str());
        glUniform4fv(loc, 1, glm::value_ptr(amb));
        flag_ambient = false;
        _needsUpdate = true;
    }

    if (flag_diffuse) {
        loc = glGetUniformLocation(_shaderID, (_lightname + ".diffuse").c_str());
        glUniform4fv(loc, 1, glm::value_ptr(diff));
        flag_diffuse = false;
        _needsUpdate = true;
    }

    if (flag_specular) {
        loc = glGetUniformLocation(_shaderID, (_lightname + ".specular").c_str());
        glUniform4fv(loc, 1, glm::value_ptr(spec));
        flag_specular = false;
        _needsUpdate = true;
    }

    if (flag_attenuation) {
        loc = glGetUniformLocation(_shaderID, (_lightname + ".constant").c_str());
        glUniform1f(loc, _constant);

        loc = glGetUniformLocation(_shaderID, (_lightname + ".linear").c_str());
        glUniform1f(loc, _linear);

        loc = glGetUniformLocation(_shaderID, (_lightname + ".quadratic").c_str());
        glUniform1f(loc, _quadratic);
        flag_attenuation = false;
        _needsUpdate = true;
    }


    loc = glGetUniformLocation(_shaderID, (_lightname + ".lightType").c_str());
    glUniform1i(loc, _type);

    if (_type == LightType::SPOT) {
        loc = glGetUniformLocation(_shaderID, (_lightname + ".direction").c_str());
        glUniform3fv(loc, 1, glm::value_ptr(_direction));

        loc = glGetUniformLocation(_shaderID, (_lightname + ".cutOff").c_str());
        glUniform1f(loc, _innerCutOff);

        loc = glGetUniformLocation(_shaderID, (_lightname + ".outerCutOff").c_str());
        glUniform1f(loc, _outerCutOff);
    }
    _displayOn = displayon;
    if (_displayOn) { // 設定代表光源物件的顯示
        _lightObj.setupVertexAttributes();
        _lightObj.setShaderID(_shaderID);
        _lightObj.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
        _lightObj.setPos(_position);
    }
}

void CLight::updateToShader()
{
    if (!_needsUpdate) return;

    GLint loc = glGetUniformLocation(_shaderID, (_lightname + ".position").c_str());
    glUniform3fv(loc, 1, glm::value_ptr(_position));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".ambient").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(_ambient));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".diffuse").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(_diffuse));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".specular").c_str());
    glUniform4fv(loc, 1, glm::value_ptr(_specular));

    loc = glGetUniformLocation(_shaderID, (_lightname + ".constant").c_str());
    glUniform1f(loc, _constant);

    loc = glGetUniformLocation(_shaderID, (_lightname + ".linear").c_str());
    glUniform1f(loc, _linear);

    loc = glGetUniformLocation(_shaderID, (_lightname + ".quadratic").c_str());
    glUniform1f(loc, _quadratic);

    loc = glGetUniformLocation(_shaderID, (_lightname + ".lightType").c_str());
    glUniform1i(loc, _type);

    if (_type == LightType::SPOT) {
        loc = glGetUniformLocation(_shaderID, (_lightname + ".direction").c_str());
        glUniform3fv(loc, 1, glm::value_ptr(_direction));

        loc = glGetUniformLocation(_shaderID, (_lightname + ".cutOff").c_str());
        glUniform1f(loc, _innerCutOff);

        loc = glGetUniformLocation(_shaderID, (_lightname + ".outerCutOff").c_str());
        glUniform1f(loc, _outerCutOff);

        loc = glGetUniformLocation(_shaderID, (_lightname + ".exponent").c_str());
        glUniform1f(loc, _exponent);
    }

    _needsUpdate = false; // 每次更新後重設為 false
}

glm::vec3 CLight::getTarget() { return _target; }
glm::vec3 CLight::getDirection() { return _direction; }

void CLight::setTarget(const glm::vec3 & target) {
    _target = target;
    _direction = glm::normalize(_target - _position);  // 計算 spot light 的照明方向
    _type = LightType::SPOT; // 設定成 spot light
}

void CLight::setCutOffDeg(float innerDeg, float outerDeg, float exponent) {
    _innerCutOff = glm::cos(glm::radians(innerDeg));
    _outerCutOff = glm::cos(glm::radians(outerDeg));
    _exponent = exponent;
    _type = LightType::SPOT;
}

void CLight::update(float dt)
{
    if (_lighingOn == false && _lighingOnflag) {
        re_amb = _ambient;
        re_diff = _diffuse;
        re_spec = _specular;

        glm::vec4 amb = _lighingOn ? _ambient : glm::vec4(0.0f);
        glm::vec4 diff = _lighingOn ? _diffuse : glm::vec4(0.0f);
        glm::vec4 spec = _lighingOn ? _specular : glm::vec4(0.0f);

        setAmbient(amb);
        setDiffuse(diff);
        setSpecular(spec);
        _lighingOnflag = false;
        _needsUpdate = true;
    }
    if (_lighingOn == true && _lighingOnflag) {
        setAmbient(re_amb);
        setDiffuse(re_diff);
        setSpecular(re_spec);
        _lighingOnflag = false;
        _needsUpdate = true;
    }

    if (_activeType == 2) {
        if (_motionOn) {
            _displayOn = true;
            if (!_isArrive) updateMotion(dt, _targetArrive);
            else updateMotion2(dt, _targetCenter);
        }
        if (_wingsRotateFlag == false) _wingsRotate += 80.0f * dt;
        else _wingsRotate -= 80.0f * dt;

        if (_wingsRotate >= 40.0f) _wingsRotateFlag = true;
        if (_wingsRotate <= -40.0f) _wingsRotateFlag = false;

        _wing[0].setRotate(_wingsRotate * 1.5f, glm::vec3(0, 0, 1));
        _wing[0].setPos(glm::vec3(_position.x + 0.4f, _position.y, _position.z));

        _wing[1].setRotate(_wingsRotate * -1.5f, glm::vec3(0, 0, 1));
        _wing[1].setPos(glm::vec3(_position.x - 0.4f, _position.y, _position.z));

        float yOffset = _wingsRotate / 160.0f;

        glm::vec3 basePos = getPos(); // 固定位置
        _lightObj.setPos(glm::vec3(basePos.x, basePos.y - yOffset, basePos.z));
    }

    if (_activeType == 1) {
        if (1) updateMotion2(dt, _targetCenter);
    }
}

// 光源運動的計算 
void CLight::updateMotion2(float dt, glm::vec3 center)
{
    //glm::vec3 center(-18.0f, 0.0f, -18.0f); // 中心點
    // 初始偏移向量，從 center 指向 _posStart
    glm::vec3 offset = _posStart - center;

    // 時間累積（建議你用 total time 代替循環）
    _clock += dt;

    // 建立旋轉矩陣（繞 Y 軸）
    float angle = _clock * glm::two_pi<float>() / 10.0f; // 10 秒轉一圈
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));

    // 旋轉 offset 向量（注意 w = 0，因為 offset 是向量）
    glm::vec3 rotatedOffset = glm::vec3(rot * glm::vec4(offset, 0.0f));

    // 新的位置 = 中心點 + 旋轉後的偏移量
    _position = center + rotatedOffset;

    // 4. 計算運動方向（本幀與上一幀差）
    glm::vec3 moveDir = glm::normalize(_position - _prevPos);
    float tailDist = 0.3f; // 尾巴距離

    glm::vec3 tailPos = _position - moveDir * tailDist;

    // 5. 更新光源與尾巴
    setPos(_position);
    if (_displayOn) _lightObj.setPos(_position);
    _wing[0].setPos(tailPos);

    setPos(_position);
    _prevPos = _position;
    if (_displayOn) _lightObj.setPos(_position);
    _needsUpdate = true;
}

void CLight::updateMotion(float dt, glm::vec3 center)
{
    _targetPos = center;

    // 計算朝向目標的單位向量
    glm::vec3 dir = glm::normalize(_targetPos - _position);

    // 移動距離 = 速度 × 時間
    float distance = _moveSpeed * dt;

    // 若距離超過目標，直接抵達目標；否則移動
    if (glm::length(_targetPos - _position) <= distance) {
        _position = _targetPos;
        _isArrive = true;
    }
    else {
        _position += dir * distance;
    }

    setPos(_position);
    if (_displayOn) _lightObj.setPos(_position);
    _posStart = _position;
    _needsUpdate = true;
}


void CLight::draw()
{
    if (_displayOn) _lightObj.draw(); // 顯示代表光源的模型
}

void CLight::drawRaw()
{
    if (_displayOn) _lightObj.drawRaw(); // 顯示代表光源的模型
}

void CLight::setActiveType(int type) {
    _activeType = type;
}

void CLight::setLightObjScale(glm::vec3 scale) {
    _lightObj.setScale(scale);
}

void CLight::setLightLevel(int level) {
    switch (level) {
    case 0: // Low
        setAmbient(glm::vec4(0.1f, 0.1f, 0.05f, 1.0f));
        setDiffuse(glm::vec4(0.3f, 0.2f, 0.1f, 1.0f));
        setIntensity(0.5f);
        setAttenuation(1.0f, 0.7f, 0.14f);
        break;
    case 1: // Medium
        setAmbient(glm::vec4(0.2f, 0.15f, 0.08f, 1.0f));
        setDiffuse(glm::vec4(0.6f, 0.5f, 0.25f, 1.0f));
        setIntensity(0.9f);
        setAttenuation(1.0f, 0.35f, 0.07f);
        break;
    case 2: // High
    default:
        setAmbient(glm::vec4(0.4f, 0.3f, 0.15f, 1.0f));
        setDiffuse(glm::vec4(1.0f, 0.7f, 0.3f, 1.0f));
        setIntensity(2.5f);
        setAttenuation(1.0f, 0.3f, 0.032f);
        break;
    }
    _needsUpdate = true;
}

void CLight::setTargetCenter(glm::vec3 target) {
    _targetCenter = target;
}

void CLight::setTargetArrive(glm::vec3 target) {
    _targetArrive = target;
}

bool CLight::getMotionOn() {
    return(_motionOn);
}

bool CLight::getisArrive() {
    return(_isArrive);
}