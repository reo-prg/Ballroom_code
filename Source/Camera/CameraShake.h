#pragma once
#include <random>
#include "../Math/VectorInterface.h"

enum class ShakeType
{
	Random,
	SinWave,
};

struct ShakeDescriptor
{
	// ����
	Vector3<float> strength_;
	
	// �h��̎��
	ShakeType type_;

	// �h��̎���
	float totalTime_;

	// ���g��(sin�g�̏ꍇ�̂�)
	float freq_;

	// �h���܂łɂ����鎞��
	float startBlend_;

	// �キ�Ȃ�Ȃ��犮�S�Ɏ~�܂�̂ɂ����鎞��
	float endBlend_;
};

class Camera;
class CameraShake
{
public:
	CameraShake();
	~CameraShake() = default;

	void Update(float deltaTime);

	/// <summary>
	/// �J������h�炷
	/// </summary>
	/// <param name="desc">�h��̏��</param>
	void StartShake(const ShakeDescriptor& desc);

	/// <summary>
	/// �h��̏��̍쐬
	/// </summary>
	/// <param name="strength">����</param>
	/// <param name="type">�h��̎��</param>
	/// <param name="totalTime">����</param>
	/// <param name="freqency">�h��̎��g��</param>
	/// <param name="startBlendTime">�h�ꂪ�����Ȃ�܂łɂ����鎞��</param>
	/// <param name="endBlendTime">�h�ꂪ���܂�̂ɂ����鎞��</param>
	/// <returns></returns>
	static ShakeDescriptor MakeShakeDescriptor(const Vector3<float>& strength, ShakeType type,
		float totalTime, float freqency, float startBlendTime, float endBlendTime)
	{
		return ShakeDescriptor{ strength, type, totalTime, freqency, startBlendTime, endBlendTime };
	}

	Vector3<float> GetOffset(void) { return offset_; }

	/// <summary>
	/// �J�������Z�b�g����
	/// </summary>
	void SetCamera(Camera* camera);
private:
	void Idle(float deltaTime);
	void Random(float deltaTime);
	void SinWave(float deltaTime);
	void (CameraShake::* update_)(float);

	Vector3<float> offset_;
	float time_;

	ShakeDescriptor desc_;

	std::mt19937 mt_;

	Vector3<float> target_;
	Vector3<float> current_;
	float freqTime_;

	Camera* camera_ = nullptr;
};
