#include "CameraShake.h"

void CameraShake::Update(float deltaTime)
{
	if (!camera_) { return; }
	(this->*update_)(deltaTime);
}

void CameraShake::StartShake(const ShakeDescriptor& desc)
{
	desc_ = desc;
	time_ = 0.0;
	switch (desc_.type_)
	{
	case ShakeType::Random:
		update_ = &CameraShake::Random;
		freqTime_ = 1.0f / desc_.freq_;
		current_ = offset_;
		break;
	case ShakeType::SinWave:
		update_ = &CameraShake::SinWave;
		break;
	default:
		break;
	}
}

CameraShake::CameraShake()
{
	update_ = &CameraShake::Idle;
	offset_ = { 0.0f, 0.0f, 0.0f };
	time_ = 0.0;
	desc_ = {};
	freqTime_ = 0.0;

	std::random_device rd;
	mt_ = std::mt19937(rd());
}

void CameraShake::SetCamera(Camera* camera)
{
	camera_ = camera;
}

void CameraShake::Idle(float deltaTime)
{
}

void CameraShake::Random(float deltaTime)
{
	time_ += deltaTime;
	if (time_ > desc_.totalTime_)
	{
		update_ = &CameraShake::Idle;
		offset_ = { 0.0f, 0.0f, 0.0f };
		return;
	}

	freqTime_ += deltaTime;
	float ft = freqTime_ / (1.0 / desc_.freq_);
	if (ft >= 1.0)
	{
		freqTime_ = 0.0;
		ft = 0.0;

		current_ = target_;

		std::uniform_real_distribution<float> dist(-1.0, 1.0);

		float x = dist(mt_);
		float y = dist(mt_);
		float z = dist(mt_);

		if (time_ < desc_.startBlend_)
		{
			x = x * (time_ / desc_.startBlend_);
			y = y * (time_ / desc_.startBlend_);
			z = z * (time_ / desc_.startBlend_);
		}
		else if (time_ > (desc_.totalTime_ - desc_.endBlend_))
		{
			x = x * ((desc_.totalTime_ - time_) / desc_.endBlend_);
			y = y * ((desc_.totalTime_ - time_) / desc_.endBlend_);
			z = z * ((desc_.totalTime_ - time_) / desc_.endBlend_);
		}

		target_.x = desc_.strength_.x * x;
		target_.y = desc_.strength_.y * y;
		target_.z = desc_.strength_.z * z;
	}



	offset_ = current_ + (target_ - current_) * ft;
}

void CameraShake::SinWave(float deltaTime)
{
	time_ += deltaTime;
	if (time_ > desc_.totalTime_)
	{
		update_ = &CameraShake::Idle;
		offset_ = { 0.0f, 0.0f, 0.0f };
		time_ = 0.0;
		return;
	}

	float h = sin(time_ * desc_.freq_ * (3.141592 * 2.0));

	if (time_ < desc_.startBlend_)
	{
		h = h * (time_ / desc_.startBlend_);
	}
	else if (time_ > (desc_.totalTime_ - desc_.endBlend_))
	{
		h = h * ((desc_.totalTime_ - time_) / desc_.endBlend_);
	}

	offset_ = desc_.strength_ * h;
}