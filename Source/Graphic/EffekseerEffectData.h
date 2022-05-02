#pragma once
#include <string>
#include <vector>
#include "../Math/VectorInterface.h"

enum class EffectState
{
	Stop,
	Play,
};

class EffekseerMng;
class EffekseerEffectData
{
	friend EffekseerMng;
public:
	EffekseerEffectData() = delete;
	EffekseerEffectData(const std::string& filepath, float exRate);
	~EffekseerEffectData();

	void Play(void);
	void Stop(void);

	void SetPosition(const Vector3<float>& pos);
	void SetRotation(const Vector3<float>& rot);
private:
	int filehandle_;
	int playhandle_;
	Vector3<float> pos_;
	Vector3<float> rot_;
	std::string filepath_;

	EffectState state_;
};

