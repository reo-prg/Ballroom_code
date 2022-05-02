#include "EffekseerEffectData.h"
#include <EffekseerForDXLib.h>
#include "EffekseerMng.h"

EffekseerEffectData::EffekseerEffectData(const std::string& filepath, float exRate)
	: filepath_(filepath), filehandle_(-1), playhandle_(-1), state_(EffectState::Stop)
{
	auto&& efkmng = EffekseerMng::GetInstance();
	filehandle_ = efkmng.getEffect(filepath_, filepath_, exRate);
}

EffekseerEffectData::~EffekseerEffectData()
{
	if (state_ == EffectState::Play)
	{
		EffekseerMng::GetInstance().stopEffect(this);
	}
}

void EffekseerEffectData::Play(void)
{
	auto&& efkmng = EffekseerMng::GetInstance();

	if (state_ == EffectState::Play)
	{
		efkmng.stopEffect(this);
	}

	efkmng.playEffect(this);
	SetPosition(pos_);
	SetRotation(rot_);
}

void EffekseerEffectData::Stop(void)
{
	if (state_ == EffectState::Stop) { return; }
	auto&& efkmng = EffekseerMng::GetInstance();
	efkmng.stopEffect(this);
}

void EffekseerEffectData::SetPosition(const Vector3<float>& pos)
{
	pos_ = pos;
	if (state_ == EffectState::Play)
	{
		SetPosPlayingEffekseer3DEffect(playhandle_, pos_.x, pos_.y, pos_.z);
	}
}

void EffekseerEffectData::SetRotation(const Vector3<float>& rot)
{
	rot_ = rot;
	if (state_ == EffectState::Play)
	{
		SetRotationPlayingEffekseer3DEffect(playhandle_, rot_.x, rot_.y, rot_.z);
	}
}
