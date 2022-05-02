#include "HealthUI.h"
#include <EffekseerForDXLib.h>
#include <algorithm>
#include "UIBase.h"
#include "UIImageRenderer.h"
#include "UITextRenderer.h"
#include "UIProgressBar.h"
#include "../Graphic/Animation.h"
#include "../Graphic/Drawer.h"
#include "../Graphic/ImageHandleManager.h"


namespace
{
	constexpr float progressSize_ = 274;
}

HealthUI::HealthUI(int maxHealth, int currentHealth)
	:maxHealth_(maxHealth), currentHealth_(currentHealth), UIIntegrator("Resource/UI/HealthUI.xml")
{
	Initialize();
}

void HealthUI::Update(float delta)
{
	UIIntegrator::Update(delta);

	playerAnim_->Update(delta);
}

void HealthUI::Draw(void)
{
	UIIntegrator::Draw();

	DrawerIns.Add2DDraw(playerAnim_->GetImageHandle(), 46.0f, 36.0f,
		0.0, 2.0, true, 0, DrawType::UI, true, false);
}

void HealthUI::SetHealth(int health)
{
	currentHealth_ = health;
	currentHealth_ = std::clamp(currentHealth_, 0, maxHealth_);


	float prog = static_cast<float>(currentHealth_) / static_cast<float>(maxHealth_);
	healthBar_->SetProgress(prog);
	if (prog <= 0.25f)
	{
		healthBar_->SetProgressHandle(ImageMngIns.Get("bar_red")[0]);
		playerAnim_->ChangeState("death");
	}
	else
	{
		healthBar_->SetProgressHandle(ImageMngIns.Get("bar_green")[0]);
		playerAnim_->ChangeState("idle");
	}
}

void HealthUI::Initialize(void)
{
	playerAnim_ = std::make_unique<Animation>("Resource/Animation/PlayerAnim.xml");

	healthBar_ = GetUIRefWithDownCast<UIProgressBar>("health");

	ImageMngIns.Load("bar_green", "Resource/Texture/General/Bar/ProgressBar_02/BarV2_ProgressBar.png");
	ImageMngIns.Load("bar_red", "Resource/Texture/General/Bar/ProgressBar_05/BarV5_ProgressBar.png");

	healthBar_->SetProgress(static_cast<float>(currentHealth_) / static_cast<float>(maxHealth_));
}
