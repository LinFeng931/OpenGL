#define GLM_ENABLE_EXPERIMENTAL
#include "CCamera.h"
#include <glm/gtc/matrix_transform.hpp>  // For glm::lookAt and glm::perspective
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>  
#include <iostream>
#include "typedefs.h"

using namespace glm;

// Singleton instance
CCamera& CCamera::getInstance()
{
	static CCamera instance;
	return instance;
}

CCamera::CCamera()
{
	_theta = 0; _phi = 0;	 // 角度由  updateViewCenter 來計算 
	_radius = 0.0f; // 半徑由  updateViewCenter 來計算

	_front = glm::vec3(5.0f, 5.0f, 5.0f); //初始視角
	_yaw = -90.0f;   // 水平角度，初始往 -Z
	_pitch = 0.0f;   // 垂直角度

	// Default projection type
	_type = Type::PERSPECTIVE;
	_view = glm::vec3(0.0f, 5.0f, 5.0f); // 鏡頭位置
	_center = glm::vec3(0.0f, 0.0f, 0.0f); // 目標點
	_up = glm::vec3(0.0f, 1.0f, 0.0f);
	// 預設鏡頭從 (5,5,5) 看向 (0,0,0) ，利用 updateViewMatrix 來計算
	updateViewCenter(_view, _front);
	_mxProj = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 1000.0f);
	_mxViewProj = _mxProj * _mxView;

	_bviewUpdate = true;
	_bprojUpdate = true;
}

void CCamera::updatePerspective(float fovy, float aspect, float zNear, float zFar)
{
	_mxProj = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	_type = Type::PERSPECTIVE;
	_bprojUpdate = true;
}

void CCamera::updateOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	_mxProj = glm::ortho(left, right, bottom, top, zNear, zFar);
	_type = Type::ORTHOGRAPHIC;
	_bprojUpdate = true;
}

void CCamera::processMouseMovement(float deltaX, float deltaY, float sensitivity)
{
	_theta += deltaX * sensitivity;
	_phi -= deltaY * sensitivity;

	// 限制 _phi 值避免接近 0 或 pi
	if (_phi < 0.1f) _phi = 0.1f;
	if (_phi > M_PI - 0.1f) _phi = M_PI - 0.1f;

	updateViewMatrix(_theta, _phi);
}

void CCamera::processMouseScroll(float deltaScroll, float zoomSensitivity)
{
	_radius -= deltaScroll * zoomSensitivity;
	if (_radius < 1.0f) _radius = 1.0f;  // 限制最小距離
	updateViewMatrix();
}

const glm::vec3& CCamera::getViewLocation()
{
	return _view;
}

void CCamera::updateView(const glm::vec3& view)
{
	_view = view;
	glm::vec3 offset = _view - _center;
	_radius = glm::length(offset);

	if (_radius < 1e-5f) {
		_radius = 1.0f;
		_theta = 0.0f;
		_phi = M_PI / 2.0f;  // 水平
	}
	else {
		_theta = -atan2(-offset.z, offset.x);
		_phi = acos(glm::clamp(offset.y / _radius, -1.0f, 1.0f));
	}
	updateViewMatrix();
}

void CCamera::updateCenter(const glm::vec3& center)
{
	_center = center;
	glm::vec3 offset = _view - _center;
	_radius = glm::length(offset);

	// 避免除以 0
	if (_radius < 1e-5f) {
		_radius = 1.0f;
		_theta = 0.0f;
		_phi = M_PI / 2.0f;  // 水平朝向
	}
	else {
		// Y 軸朝上，球座標計算
		_theta = -atan2(-offset.z, offset.x);
		_phi = acos(glm::clamp(offset.y / _radius, -1.0f, 1.0f));
	}
	updateViewMatrix();
}

void CCamera::updateViewCenter(const glm::vec3& view, const glm::vec3& center)
{
	_view = view; _center = center;

	glm::vec3 offset = _view - _center;
	_radius = glm::length(offset);

	// 避免除以 0
	if (_radius < 1e-5f) {
		_radius = 1.0f;
		_theta = 0.0f;
		_phi = M_PI / 2.0f;  // 水平朝向
	}
	else {
		// Y 軸朝上，球座標計算
		_theta = -atan2(-offset.z, offset.x);
		_phi = acos(glm::clamp(offset.y / _radius, -1.0f, 1.0f));
	}
	updateViewMatrix();
}

void CCamera::updateViewMatrix()
{
	_mxView = glm::lookAt(_view, _center, _up);
	_bviewUpdate = true;
}

// Convert spherical coordinates to Cartesian coordinates
void CCamera::updateViewMatrix(float theta, float phi)
{
	float next_viewx = _center.x + _radius * sin(phi) * cos(theta);
	float next_viewy = _center.y + _radius * cos(phi);
	float next_viewz = _center.z + _radius * sin(phi) * sin(theta);

	if (next_viewx > -8.0f && next_viewx < 8.0f) _view.x = _center.x + _radius * sin(phi) * cos(theta);
	if (next_viewy > -8.0f && next_viewy < 8.0f) _view.y = _center.y + _radius * cos(phi);
	if (next_viewz > -8.0f && next_viewz < 8.0f) _view.z = _center.z + _radius * sin(phi) * sin(theta);
	_mxView = glm::lookAt(_view, _center, _up);
	_bviewUpdate = true;
}

void CCamera::updateRadius(float delta) {
	_radius += delta;
	if (_radius < 1.0f) _radius = 1.0f;  // 限制最小距離
	// 更新相機位置
	_view.x = _center.x + _radius * sin(_phi) * cos(_theta);
	_view.y = _center.y + _radius * cos(_phi);
	_view.z = _center.z + _radius * sin(_phi) * sin(_theta);
	updateViewMatrix();
}

const glm::mat4& CCamera::getProjectionMatrix()
{
	return _mxProj;
}

const glm::mat4& CCamera::getViewMatrix()
{
	return _mxView;
}

const glm::mat4& CCamera::getViewProjectionMatrix() const
{
	if (_bviewUpdate || _bprojUpdate) {
		_mxViewProj = _mxProj * _mxView;
		_bviewUpdate = false;
		_bprojUpdate = false;
	}
	return _mxViewProj;
}

CCamera::Type CCamera::getProjectionType() const
{
	return _type;
}

void CCamera::processMouseMovement2(float deltaX, float deltaY, float sensitivity)
{
	// 旋轉角度（弧度）
	_yaw += deltaX * sensitivity;
	_pitch += deltaY * sensitivity;

	// 限制 pitch
	if (_pitch > 89.0f)  _pitch = 89.0f;
	if (_pitch < -89.0f) _pitch = -89.0f;

	// 根據 yaw/pitch 計算方向向量
	glm::vec3 direction;
	direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	direction.y = sin(glm::radians(_pitch));
	direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(direction);

	_center = _view + _front;
	updateViewMatrix();
}

glm::vec3 CCamera::getFront() {
	return(_front);
}

glm::vec3 CCamera::getUp() {
	return(_up);
}

void CCamera::move(const glm::vec3& newViewPos) {
	_view = newViewPos;
	_center = _view + _front;
	updateViewMatrix();
}