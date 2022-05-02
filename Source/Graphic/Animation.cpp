#include "Animation.h"
#include <algorithm>
#include "AnimationManager.h"
#include "ImageHandleManager.h"

Animation::Animation(std::string_view filename)
{
	lpAnimManager.LoadAnimation(filename);
	filePath_ = filename;

	auto& anims = lpAnimManager.GetAnimations(filePath_);

	currentTime_ = 0.0;
	currentData_ = &anims.begin()->second.imageID_.front();
	currentState_ = anims.begin()->first;
	finalTime_ = anims.begin()->second.imageID_.back().second;
	loopCount_ = anims.begin()->second.loop_;
	imagePath_ = lpAnimManager.GetImagePath(filePath_);
	imageHandle_ = ImageMngIns.Get(imagePath_)[anims.begin()->second.imageID_.front().first];
}

void Animation::SetAnimationData(std::string_view filename)
{
	if (filePath_ == filename) { return; }
	lpAnimManager.LoadAnimation(filename);
	filePath_ = filename;

	auto& anims = lpAnimManager.GetAnimations(filePath_);

	currentTime_ = 0.0;
	currentData_ = &anims.begin()->second.imageID_.front();
	currentState_ = anims.begin()->first;
	finalTime_ = anims.begin()->second.imageID_.back().second;
	loopCount_ = anims.begin()->second.loop_;
	imagePath_ = lpAnimManager.GetImagePath(filePath_);
	imageHandle_ = ImageMngIns.Get(imagePath_)[anims.begin()->second.imageID_.front().first];
}

void Animation::Update(double deltaTime)
{
	// 回数が残ってないなら処理しない
	if (!loopCount_) { return; }

	AnimData& ad = lpAnimManager.GetAnimation(filePath_, currentState_);

	currentTime_ += deltaTime;
	// 時間がオーバーしているかチェック
	// 再生
	if (currentTime_ > finalTime_)
	{
		if (loopCount_ != loopInfinity) { loopCount_--; }
		if (loopCount_)
		{
			currentTime_ = fmod(currentTime_, finalTime_);
		}
	}
	// 逆再生
	if (currentTime_ < 0.0)
	{
		if (loopCount_ != loopInfinity) { loopCount_--; }
		if (loopCount_)
		{
			currentTime_ = finalTime_ + fmod(currentTime_, finalTime_);
		}
	}

	// 時間から現在のコマを探す
	auto it = std::find_if(ad.imageID_.begin(), ad.imageID_.end(),
		[this](std::pair<int, double>& data) { return data.second > currentTime_; });

	// 無いなら最後のコマにする
	if (it == ad.imageID_.end())
	{
		--it;
	}
	// コマが変わっていないならなにもしない
	if (currentData_ == it._Ptr)
	{
		return;
	}
	currentData_ = it._Ptr;
	imageHandle_ = ImageMngIns.Get(imagePath_)[currentData_->first];
}
#include <Windows.h>
void Animation::ChangeState(std::string_view state)
{
	if (currentState_ == state){ return; }
	auto& anims = lpAnimManager.GetAnimations(filePath_);
	if (!anims.contains(std::string(state)))
	{
		return;
	}
	currentState_ = state;
	currentTime_ = 0.0;
	currentData_ = &anims.at(currentState_).imageID_.front();
	finalTime_ = anims.at(currentState_).imageID_.back().second;
	imageHandle_ = ImageMngIns.Get(imagePath_)[currentData_->first];
	loopCount_ = anims.at(currentState_).loop_;
}

int Animation::GetImageHandle(void)const
{
	return imageHandle_;
}

double Animation::GetCurrentAnimationProgress(void)const
{
	return currentTime_ / finalTime_;
}

double Animation::GetCurrentAnimationTime(void) const
{
	return currentTime_;
}

const std::string& Animation::GetCurrentState(void) const
{
	return currentState_;
}

const std::string& Animation::GetImagePath(void) const
{
	return imagePath_;
}

void Animation::SetLoopCount(int count)
{
	loopCount_ = count;
}
