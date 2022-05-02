#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "../Math/VectorInterface.h"
#include "BoardPolygon.h"

#define EffectIns EffectMng::GetInstance()

struct EffectData;
class BoardPolygon;
class EffectMng
{
public:
	// エフェクトの再生
	// key:キー		pos:座標
	// angle:角度	exrate:拡大率
	// totalTime:合計の再生時間
	void PlayEffect(const char* key, const Vector3<float>& pos, float angle, float exrate, 
		float totalTime = 1.0, bool invert_x = false, bool invert_y = false);

	// 描画(毎フレーム呼んでね)
	void Draw(float deltaTime, const Vector2<float>& offset);

	// エフェクトの読み込み
	// key:キー			filename:素材のパス
	// cnt_all:コマ数	cnt_x:横のコマ数		cnt_y:縦のコマ数
	// size_x:1コマの横サイズ	size_y:1コマの縦サイズ
	void LoadEffect(const char* key, const char* directory);

	static void Create(void)
	{
		instance_ = new EffectMng();
	}

	static EffectMng& GetInstance(void)
	{
		return *instance_;
	};

	static void Destroy(void)
	{
		delete instance_;
	}
private:
	EffectMng() = default;
	EffectMng(const EffectMng&) = delete;
	EffectMng operator=(const EffectMng&) = delete;

	std::vector<std::unique_ptr<EffectData>> playingEffect_;
	std::map<std::string, std::pair<std::string, int>> effects_;

	void Update(float deltaTime);

	static EffectMng* instance_;
};

// エフェクトの情報
struct EffectData
{
	std::string handle_;	// 画像ハンドル
	Vector3<float> pos_;		// 座標
	float angle_;				// 角度
	float exRate_;				// 拡大率
	unsigned int frame_;		// 現在のフレーム数
	unsigned int playSpeed_;	// 再生速度
	size_t count_;

	Vector2<int> size_;

	std::unique_ptr<BoardPolygon> poly_;

	Vector2<bool> inv_;

	float time_ = 0.0;
	float speed_;

	EffectData(const std::string& handle, const Vector3<float>& pos, float angle, float exrate, float playspeed, size_t count)
		:handle_(handle), pos_(pos), angle_(angle), exRate_(exrate), frame_(0), speed_(playspeed), count_(count) {};
};
