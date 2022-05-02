#include <type_traits>
#include "EffekseerMng.h"
#include "EffekseerEffectData.h"


EffekseerMng* EffekseerMng::instance_ = nullptr;

int EffekseerMng::getEffect(const std::string & key)
{
	return getEffect(key,key,1.0f);
}

int EffekseerMng::getEffect(const std::string & filename, const std::string & key, const float& exRate)
{
	if (effectMap_.find(key) == effectMap_.end())
	{
		int a = LoadEffekseerEffect(filename.c_str(), exRate);
		effectMap_.emplace(key, a);
	}
	return effectMap_[key];
}

void EffekseerMng::playEffect(EffekseerEffectData* efkData)
{
	efkData->playhandle_ = PlayEffekseer3DEffect(efkData->filehandle_);
	effectList_.emplace_back(efkData);
	efkData->state_ = EffectState::Play;
}

void EffekseerMng::stopEffect(EffekseerEffectData* efkData)
{
	auto eraseEfk = std::find_if(effectList_.begin(), effectList_.end(), [&](EffekseerEffectData* val) {return val == efkData; });
	
	if (eraseEfk == effectList_.end())
	{
		return;
	}
	
	effectList_.erase(eraseEfk);
	StopEffekseer3DEffect(efkData->playhandle_);
	efkData->state_ = EffectState::Stop;
}

void EffekseerMng::stopEffectAll(void)
{
	for (auto& data : effectList_)
	{
		StopEffekseer3DEffect(data->playhandle_);
		data->state_ = EffectState::Stop;
	}
	effectList_.clear();
}

void EffekseerMng::UpdateEffekseer(void)
{
	Effekseer_Sync3DSetting();

	for (auto data = effectList_.begin(); data != effectList_.end();)
	{
		auto& d = (*data);
		if (IsEffekseer3DEffectPlaying(d->playhandle_) == -1)
		{
			StopEffekseer3DEffect(d->playhandle_);
			d->state_ = EffectState::Stop;
			data = effectList_.erase(data);
		}
		else
		{
			//SetPosPlayingEffekseer3DEffect(d->playhandle_,
			//	static_cast<float>(*(pos.first) - static_cast<int>(lpSceneMng.GetcPos().x) + lpSceneMng.ScreenSize.x / 2  + offset.first * (static_cast<int>(*dir) - 1)),
			//	static_cast<float>(*(pos.second) - static_cast<int>(lpSceneMng.GetcPos().y) + lpSceneMng.ScreenSize.y / 2 + offset.second), 0);
			data++;
		}
	}
	UpdateEffekseer3D();
	
	DrawEffekseer3D();
	
	//for (auto& data : effectList_)
	//{
	//	DrawEffekseer3D_Draw(data->playhandle_);
	//}
}

void EffekseerMng::SetPlaySpeed(float speed)
{
	for (auto& data : effectList_)
	{
		SetSpeedPlayingEffekseer3DEffect(data->playhandle_, speed);
	}
}

EffekseerMng::EffekseerMng()
{
	if (Effekseer_Init(8000) == -1)
	{
		DebugBreak();
	}
	SetChangeScreenModeGraphicsSystemResetFlag(false);
}


EffekseerMng::~EffekseerMng()
{
	stopEffectAll();
	for (auto& data : effectMap_)
	{
		DeleteEffekseerEffect(data.second);
	}
	Effkseer_End();
}

