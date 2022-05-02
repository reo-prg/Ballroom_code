#include "UIProgressBar.h"
#include "../Graphic/Drawer.h"
#include "../Graphic/ImageHandleManager.h"

UIProgressBar::UIProgressBar(const Vector2<float>& pos, const std::string& basePath, 
	const std::string& progressPath, const std::string& id, int priority)
	: id_(id), priority_(priority)
{
	pos_ = pos;
	Initialize(basePath, progressPath);
}

UIProgressBar::UIProgressBar(const Vector2<float>& pos, const std::string& basePath, 
	const std::string& progressPath, const std::string& divPath, const std::string& id, int priority)
	: id_(id), priority_(priority)
{
	pos_ = pos;
	Initialize(basePath, progressPath, divPath);
}

UIProgressBar::~UIProgressBar()
{
}

void UIProgressBar::Draw(void)
{
	auto&& inst = Drawer::GetInstance();
	int pr = priority_;

	inst.Add2DDraw(baseHandle_, pos_.x, pos_.y, 0.0, scale_, true, pr + 2, DrawType::UI);

	inst.Add2DDraw([this]() 
		{
			DrawRectGraphF(pos_.x - barWidth_ / 2, pos_.y - barHeight_ / 2, 0, 0,
				static_cast<int>(static_cast<float>(barWidth_) * progress_),
				barHeight_, progressHandle_, true);
		}, priority_, DrawType::UI);

	if (divHandle_ != -1)
	{
		inst.Add2DDraw(divHandle_, pos_.x, pos_.y, 0.0, scale_, true, pr, DrawType::UI);
	}
}

void UIProgressBar::AddProgress(float add)
{
	progress_ = std::clamp(progress_ + add, 0.0f, 1.0f);
}

void UIProgressBar::SetProgress(float prog)
{
	progress_ = prog;
}

void UIProgressBar::Initialize(const std::string& basePath, const std::string& progressPath)
{
	auto&& inst = ImageHandleManager::GetInstance();
	inst.Load(basePath);
	inst.Load(progressPath);

	baseHandle_ = inst.Get(basePath)[0];
	progressHandle_ = inst.Get(progressPath)[0];

	GetGraphSize(progressHandle_, &barWidth_, &barHeight_);

	scale_ = 1.0f;
}

void UIProgressBar::Initialize(const std::string& basePath, const std::string& progressPath, const std::string& divPath)
{
	auto&& inst = ImageHandleManager::GetInstance();
	inst.Load(basePath);
	inst.Load(progressPath);
	inst.Load(divPath);

	baseHandle_ = inst.Get(basePath)[0];
	progressHandle_ = inst.Get(progressPath)[0];
	divHandle_ = inst.Get(divPath)[0];

	int h;
	GetGraphSize(progressHandle_, &barWidth_, &barHeight_);

	scale_ = 1.0f;
}
