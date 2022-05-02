#include "SettingScene.h"
#include "../Actor/Player.h"
#include "../Audio/AudioManager.h"
#include "../Audio/SubmixHandleManager.h"
#include "../Graphic/Animation.h"
#include "../Graphic/Camera.h"
#include "../Graphic/Drawer.h"
#include "../Map/BackGround.h"
#include "../UI/ExpandUI/SettingUI.h"
#include "../Application.h"
#include "Transition/Fade.h"
#include "SceneManager.h"
#include "TitleScene.h"

namespace
{
	constexpr float SelectBGSpeed = 300.0f;

	constexpr float StartPlayerSpeed = 300.0f;
	constexpr float BackPlayerSpeed = -300.0f;
	constexpr float EndPlayerSpeed = 700.0f;
	constexpr float EndDuration = 1.0f;
}

SettingScene::SettingScene()
{
}

void SettingScene::Update(float delta)
{
	(this->*update_)(delta);
}

void SettingScene::BeforeDraw(void)
{
	bg_->Draw();
}

void SettingScene::Draw(void)
{
	DrawerIns.Add2DDraw(playerAnim_->GetImageHandle(), playerPos_.x, playerPos_.y,
		0.0, 4.0, true, 500, DrawType::Normal, playerGoForward_, false);

	ui_->Draw();
}

void SettingScene::Initialize(void)
{
	camera_ = std::make_unique<Camera>();

	camera_->SetCameraPosition(400.0f, 300.0f, -800.0f);
	camera_->AddAngleDeg(0.0f, 0.0f, 0.0f);

	bg_ = std::make_unique<BackGround>();
	bg_->Load("Resource/BG/ForestBG.xml");
	bg_->SetCamera(camera_.get());

	auto&& [w, h] = Application::GetWindowSize();
	screenWidth_ = w;

	playerPos_ = { w + 100.0f, h * 0.75f };

	playerAnim_ = std::make_unique<Animation>("Resource/Animation/PlayerAnim.xml");

	ui_ = std::make_unique<SettingUI>("Resource/UI/SettingSceneUI.xml", this);
	ui_->SetVisible(true);

	update_ = &SettingScene::StartUpdate;

	AudioIns.LoadSound("Resource/Sound/BGM134-121031-honobonohanabana-wav.wav", "setting_bgm");
	//bgmHandle_ = AudioIns.Play("setting_bgm", 0.5f, true, { SubmixHandleMng.GetHandle("bgm") });
	bgmHandle_ = AudioIns.PlayLoop("setting_bgm", 0.0f, 0.0f, XAUDIO2_LOOP_INFINITE, 
		0.5f, true, { SubmixHandleMng.GetHandle("bgm") });

	playerGoForward_ = false;
	playerSpeed_ = BackPlayerSpeed;
}

void SettingScene::StartUpdate(float delta)
{
	playerAnim_->Update(delta);

	bg_->Update(delta);

	ui_->Update(delta);

	playerPos_.x += playerSpeed_ * delta;

	AudioIns.SetVolume(bgmHandle_, std::clamp(0.5f * (1.0f - ((playerPos_.x - screenWidth_ * 0.7f) / (screenWidth_ * 0.3f))) + 0.5f, 0.0f, 1.0f));

	if (playerPos_.x <= screenWidth_ * 0.7f)
	{
		playerPos_.x = static_cast<float>(screenWidth_ * 0.7f);
		ui_->SetVisible(true);
		update_ = &SettingScene::SelectUpdate;
	}
}

void SettingScene::SelectUpdate(float delta)
{
	playerAnim_->Update(delta);

	bg_->Update(delta);

	ui_->Update(delta);

	if (ui_->GetState() == SettingUI::State::BACK)
	{
		playerSpeed_ = EndPlayerSpeed;
		playerGoForward_ = true;
		update_ = &SettingScene::EndUpdate;
		time_ = 0.0f;
	}
}

void SettingScene::EndUpdate(float delta)
{
	playerAnim_->Update(delta);
	bg_->Update(delta);

	playerPos_.x += playerSpeed_ * delta;
	ui_->Update(delta);

	time_ += delta;

	AudioIns.SetVolume(bgmHandle_, std::clamp(1.0f - (time_ / EndDuration), 0.0f, 1.0f));
	if (time_ > EndDuration)
	{
		AudioIns.Stop(bgmHandle_);
		mng_->ChangeScene(new Fade(mng_->GetCurrentScene(), std::make_shared<TitleScene>(false), 0.5f, 0.5f));
	}
}
