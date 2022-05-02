#include "Camera.h"
#include <algorithm>
#include <cmath>
#include <string>
#include "../Math/MathUtility.h"


Camera::Camera()
{
	position_ = { 0.0f, 0.0f, 0.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
}

Camera::~Camera()
{
}

void Camera::Update(void)
{

}

void Camera::SetClipping(float c_near, float c_far)
{
	SetCameraNearFar(c_near, c_far);
}

void Camera::SetAngleDeg(float x, float y, float z)
{
	rotate_.x = DegreeToRadian(x);
	rotate_.y = DegreeToRadian(y);
	rotate_.z = DegreeToRadian(z);
}

void Camera::AddAngleDeg(float x, float y, float z)
{
	rotate_.x += DegreeToRadian(x);
	rotate_.y += DegreeToRadian(y);
	rotate_.z += DegreeToRadian(z);
}

void Camera::SetAngleRad(float x, float y, float z)
{
	rotate_.x = x;
	rotate_.y = y;
	rotate_.z = z;
}

void Camera::AddAngleRad(float x, float y, float z)
{
	rotate_.x += x;
	rotate_.y += y;
	rotate_.z += z;
}

VECTOR Camera::GetCameraAngle(void)
{
	return { rotate_.x, rotate_.y, rotate_.z };
}

void Camera::SetCameraPosition(float x, float y, float z)
{
	position_.x = x;
	position_.y = y;
	position_.z = z;
}

void Camera::AddCameraPosition(float x, float y, float z)
{
	position_.x += x;
	position_.y += y;
	position_.z += z;

	target_.x += x;
	target_.y += y;
	target_.z += z;
}

void Camera::SetPositionX(float x)
{
	position_.x = x;
}

void Camera::SetPositionY(float y)
{
	position_.y = y;
}

void Camera::SetPositionZ(float z)
{
	position_.z = z;
}

void Camera::SetCameraTarget(float x, float y, float z)
{
	//target_ = { x, y, z };

	Vector3<float> vec = Vector3<float>{ x, y, z } - Vector3<float>{ position_.x, position_.y, position_.z };

	rotate_.y = atan2(vec.z, vec.x);
	rotate_.x = sinf(vec.y);
}

void Camera::SetCamera(const Vector3<float>& offset)
{
	SetCameraPositionAndAngle(VAdd(position_, { offset.x, offset.y, offset.z }), rotate_.x, rotate_.y, rotate_.z);
	//SetCameraPositionAndTargetAndUpVec(position_, target_, { 0.0f, 1.0f, 0.0f });
}
