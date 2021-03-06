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
	// 強さ
	Vector3<float> strength_;
	
	// 揺れの種類
	ShakeType type_;

	// 揺れの時間
	float totalTime_;

	// 周波数(sin波の場合のみ)
	float freq_;

	// 揺れるまでにかける時間
	float startBlend_;

	// 弱くなりながら完全に止まるのにかける時間
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
	/// カメラを揺らす
	/// </summary>
	/// <param name="desc">揺れの情報</param>
	void StartShake(const ShakeDescriptor& desc);

	/// <summary>
	/// 揺れの情報の作成
	/// </summary>
	/// <param name="strength">強さ</param>
	/// <param name="type">揺れの種類</param>
	/// <param name="totalTime">時間</param>
	/// <param name="freqency">揺れの周波数</param>
	/// <param name="startBlendTime">揺れが強くなるまでにかける時間</param>
	/// <param name="endBlendTime">揺れが収まるのにかける時間</param>
	/// <returns></returns>
	static ShakeDescriptor MakeShakeDescriptor(const Vector3<float>& strength, ShakeType type,
		float totalTime, float freqency, float startBlendTime, float endBlendTime)
	{
		return ShakeDescriptor{ strength, type, totalTime, freqency, startBlendTime, endBlendTime };
	}

	Vector3<float> GetOffset(void) { return offset_; }

	/// <summary>
	/// カメラをセットする
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
