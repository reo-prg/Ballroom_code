#include "EffectMng.h"
#include <DxLib.h>
#include <algorithm>
#include <filesystem>
#include "BoardPolygon.h"
#include "ImageHandleManager.h"
#include "Drawer.h"

EffectMng* EffectMng::instance_ = nullptr;

void EffectMng::Update(float deltaTime)
{
	for (auto& eff : playingEffect_)
	{
		eff->frame_++;
		eff->time_ += deltaTime;
	}

	auto it = std::remove_if(playingEffect_.begin(), playingEffect_.end(),
		[](std::unique_ptr<EffectData>& ef) { return ef->time_ >= (static_cast<float>(ef->count_) * ef->speed_); });
	playingEffect_.erase(it, playingEffect_.end());

	auto&& inst = ImageHandleManager::GetInstance();
	for (auto& eff : playingEffect_)
	{
		eff->poly_->SetTexture(inst.Get(eff->handle_)[static_cast<int>(eff->time_ / eff->speed_)]);
	}
}

void EffectMng::PlayEffect(const char* key, const Vector3<float>& pos, float angle, float exrate, float totalTime,
	bool invert_x, bool invert_y)
{
	if (effects_.find(key) == effects_.end())
	{
		return;
	}
	EffectData* eff = new EffectData(effects_[key].first, pos, angle, exrate, totalTime / static_cast<float>(effects_[key].second), 
		effects_[key].second);
	eff->poly_ = std::make_unique<BoardPolygon>();
	int h = ImageHandleManager::GetInstance().Get(effects_[key].first)[0];
	GetGraphSize(h, &eff->size_.x, &eff->size_.y);
	eff->size_ = eff->size_ * exrate;
	eff->poly_->SetScale({ static_cast<float>(eff->size_.x), static_cast<float>(eff->size_.y) });
	eff->poly_->SetPosition({ eff->pos_.x, eff->pos_.y, eff->pos_.z });
	eff->inv_ = { invert_x, invert_y };
	eff->poly_->TextureInvertX(invert_x);
	eff->poly_->TextureInvertY(invert_y);
	playingEffect_.emplace_back(std::unique_ptr<EffectData>(eff));
}

void EffectMng::Draw(float deltaTime, const Vector2<float>& offset)
{
	Update(deltaTime);

	for (const auto& eff : playingEffect_)
	{
		//DrawRotaGraphF(eff->pos_.x + offset.x, eff->pos_.y + offset.y, 
		//	eff->exRate_, eff->angle_, lpImageManager.Get(eff->handle_)[static_cast<int>(eff->time_ / eff->speed_)], true);
		eff->poly_->Draw();
	}
}

void EffectMng::LoadEffect(const char* key, const char* directory)
{
	if (effects_.find(key) != effects_.end())
	{
		return;
	}
	std::vector<int> handle;
	for (auto& fp : std::filesystem::directory_iterator(directory))
	{
		handle.push_back(LoadGraph(fp.path().string().c_str()));
	}

	ImageHandleManager::GetInstance().Register(directory, handle);

	effects_.emplace(key, std::pair{ directory, handle.size() });
}
