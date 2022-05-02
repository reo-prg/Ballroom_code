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
	// ‹­‚³
	Vector3<float> strength_;
	
	// —h‚ê‚Ìí—Ş
	ShakeType type_;

	// —h‚ê‚ÌŠÔ
	float totalTime_;

	// ü”g”(sin”g‚Ìê‡‚Ì‚İ)
	float freq_;

	// —h‚ê‚é‚Ü‚Å‚É‚©‚¯‚éŠÔ
	float startBlend_;

	// ã‚­‚È‚è‚È‚ª‚çŠ®‘S‚É~‚Ü‚é‚Ì‚É‚©‚¯‚éŠÔ
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
	/// ƒJƒƒ‰‚ğ—h‚ç‚·
	/// </summary>
	/// <param name="desc">—h‚ê‚Ìî•ñ</param>
	void StartShake(const ShakeDescriptor& desc);

	/// <summary>
	/// —h‚ê‚Ìî•ñ‚Ìì¬
	/// </summary>
	/// <param name="strength">‹­‚³</param>
	/// <param name="type">—h‚ê‚Ìí—Ş</param>
	/// <param name="totalTime">ŠÔ</param>
	/// <param name="freqency">—h‚ê‚Ìü”g”</param>
	/// <param name="startBlendTime">—h‚ê‚ª‹­‚­‚È‚é‚Ü‚Å‚É‚©‚¯‚éŠÔ</param>
	/// <param name="endBlendTime">—h‚ê‚ªû‚Ü‚é‚Ì‚É‚©‚¯‚éŠÔ</param>
	/// <returns></returns>
	static ShakeDescriptor MakeShakeDescriptor(const Vector3<float>& strength, ShakeType type,
		float totalTime, float freqency, float startBlendTime, float endBlendTime)
	{
		return ShakeDescriptor{ strength, type, totalTime, freqency, startBlendTime, endBlendTime };
	}

	Vector3<float> GetOffset(void) { return offset_; }

	/// <summary>
	/// ƒJƒƒ‰‚ğƒZƒbƒg‚·‚é
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
