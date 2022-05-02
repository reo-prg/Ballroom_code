#pragma once
#include <EffekseerForDXLib.h>
#include "../Math/VectorInterface.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Update(void);

	void SetClipping(float c_near, float c_far);

	void SetAngleDeg(float x, float y, float z);
	void AddAngleDeg(float x, float y, float z);
	void SetAngleRad(float x, float y, float z);
	void AddAngleRad(float x, float y, float z);

	VECTOR GetCameraAngle(void);

	void SetCameraPosition(float x, float y, float z);
	void AddCameraPosition(float x, float y, float z);

	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPositionZ(float z);

	void SetCameraTarget(float x, float y, float z);

	VECTOR GetCameraPosition(void) { return position_; }
	Vector3<float> GetCameraRotate(void) { return rotate_; }

	void SetCamera(const Vector3<float>& offset);
private:
	VECTOR position_;
	VECTOR target_;
	Vector3<float> rotate_;
};

