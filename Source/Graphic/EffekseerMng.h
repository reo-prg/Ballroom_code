#pragma once
#include <EffekseerForDXLib.h>
#include <map>
#include <memory>
#include <vector>
#include <tuple>
#include "../Math/VectorInterface.h"

#define lpEffectMng EffekseerMng::GetInstance()

class EffekseerEffectData;

class EffekseerMng
{
public:
	static void Create(void)
	{
		instance_ = new EffekseerMng();
	}

	static EffekseerMng& GetInstance(void)
	{
		return *instance_;
	}

	static void Destroy(void)
	{
		delete instance_;
		instance_ = nullptr;
	}

	int getEffect(const std::string& key);														// キーを渡すとエフェクトのハンドルを返す
	int getEffect(const std::string& filename, const std::string& key, const float& exRate);	// ファイル名で読み込み、キーに入れる

	/// <summary>
	/// エフェクトを再生する
	/// </summary>
	/// <param name="efkData">データ</param>
	void playEffect(EffekseerEffectData* efkData);			
	void stopEffect(EffekseerEffectData* efkData);

	/// <summary>
	/// すべてのエフェクトを停止する
	/// </summary>
	void stopEffectAll(void);

	void UpdateEffekseer(void);

	void SetPlaySpeed(float speed);
private:
	static EffekseerMng* instance_;

	std::map<std::string, int> effectMap_;
	
	std::vector<EffekseerEffectData*> effectList_;	

	EffekseerMng();
	EffekseerMng(const EffekseerMng&) = delete;
	EffekseerMng operator=(const EffekseerMng&) = delete;
	~EffekseerMng();
};

