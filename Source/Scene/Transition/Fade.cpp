#include "Fade.h"
#include "../../Graphic/Drawer.h"
#include "../../Graphic/ShaderHandleManager.h"
#include "../SceneManager.h"

Fade::Fade(const std::shared_ptr<Scene>& prevScene, const std::shared_ptr<Scene>& nextScene,
	float fadeoutDuration, float fadeinDuration)
	: Transition(prevScene, nextScene), outDur_(fadeoutDuration), inDur_(fadeinDuration)
{
}

Fade::~Fade()
{
}

void Fade::Update(float delta)
{
	timer_ += delta;
	(this->*update_)(delta);
}

void Fade::Draw(void)
{
	(this->*draw_)();
}

void Fade::Initialize(void)
{
	update_ = &Fade::FadeOut;
	draw_ = &Fade::PrevDraw;
	timer_ = 0.0f;

	ShaderMngIns.LoadPS("panel", "Resource/Shader/PanelFade.pso");

	DrawerIns.SetFadeShader(ShaderMngIns.Get("panel"));
	DrawerIns.SetFadeParameter(0.0f, 0, 0);

	GetPrevScreen();
}

void Fade::FadeOut(float delta)
{
	progress_ = timer_ / outDur_;
	if (timer_ >= outDur_)
	{
		prev_.reset();
		next_->SetManager(mng_);
		next_->Initialize();
		GetNextScreen();
		update_ = &Fade::FadeIn;
		draw_ = &Fade::NextDraw;
		timer_ = 0.0f;
		DrawerIns.SetFadeParameter(1.0f, 0, 0);
		DrawerIns.SetDamageEffect(0.0f, true);
		resetTime_ = true;
	}
}

void Fade::FadeIn(float delta)
{
	if (resetTime_)
	{
		resetTime_ = false;
		return;
	}
	progress_ = 1.0f - timer_ / inDur_;
	if (timer_ >= inDur_)
	{
		DrawerIns.SetFadeProgress(0.0f);
		mng_->ChangeScene(next_, false);
	}
}

void Fade::PrevDraw(void)
{
	DrawerIns.Add2DDraw([this]() { DrawGraph(0, 0, prevScreen_, true); }, 0, DrawType::Normal);
	//DrawerIns.Add2DDraw([this]() {
	//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * progress_));
	//	DrawBox(0, 0, screenWidth_, screenHeight_, 0x000000, true);
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//	}, 0, DrawType::UI);

	DrawerIns.SetFadeProgress(progress_);
}

void Fade::NextDraw(void)
{
	DrawerIns.Add2DDraw([this]() { DrawGraph(0, 0, nextScreen_, true); }, 0, DrawType::Normal);
	//DrawerIns.Add2DDraw([this]() {
	//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * progress_));
	//	DrawBox(0, 0, screenWidth_, screenHeight_, 0x000000, true);
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//	}, 0, DrawType::UI);
	DrawerIns.SetFadeProgress(progress_);
}
