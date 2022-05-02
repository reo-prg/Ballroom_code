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
	// �񐔂��c���ĂȂ��Ȃ珈�����Ȃ�
	if (!loopCount_) { return; }

	AnimData& ad = lpAnimManager.GetAnimation(filePath_, currentState_);

	currentTime_ += deltaTime;
	// ���Ԃ��I�[�o�[���Ă��邩�`�F�b�N
	// �Đ�
	if (currentTime_ > finalTime_)
	{
		if (loopCount_ != loopInfinity) { loopCount_--; }
		if (loopCount_)
		{
			currentTime_ = fmod(currentTime_, finalTime_);
		}
	}
	// �t�Đ�
	if (currentTime_ < 0.0)
	{
		if (loopCount_ != loopInfinity) { loopCount_--; }
		if (loopCount_)
		{
			currentTime_ = finalTime_ + fmod(currentTime_, finalTime_);
		}
	}

	// ���Ԃ��猻�݂̃R�}��T��
	auto it = std::find_if(ad.imageID_.begin(), ad.imageID_.end(),
		[this](std::pair<int, double>& data) { return data.second > currentTime_; });

	// �����Ȃ�Ō�̃R�}�ɂ���
	if (it == ad.imageID_.end())
	{
		--it;
	}
	// �R�}���ς���Ă��Ȃ��Ȃ�Ȃɂ����Ȃ�
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
