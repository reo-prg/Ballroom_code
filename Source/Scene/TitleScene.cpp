#include "TitleScene.h"
#include "../Input/InputIntegrator.h"
#include "MainScene.h"
#include "SceneManager.h"
#include "Transition/Fade.h"
#include "../UI/ExpandUI/TitleUI.h"
#include "../UI/ExpandUI/ExitUI.h"
#include "../Audio/AudioManager.h"
#include "../Audio/SubmixHandleManager.h"
#include "../Graphic/Camera.h"
#include "../Map/BackGround.h"
#include "../Graphic/Animation.h"
#include "../Graphic/Drawer.h"
#include "../Application.h"
#include "StageSelectScene.h"
#include "SettingScene.h"

namespace
{
	constexpr float SelectBGSpeed = 300.0f;

	constexpr float StartPlayerSpeed = 300.0f;
	constexpr float BackPlayerSpeed = -700.0f;
	constexpr float EndPlayerSpeed = 700.0f;
	constexpr float EndDuration = 1.0f;
}

TitleScene::TitleScene(bool isBackFromAnotherScene)
	: isBack_(isBackFromAnotherScene), playerGoForward_(!isBackFromAnotherScene)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update(float delta)
{
	(this->*update_)(delta);
}

void TitleScene::BeforeDraw(void)
{
	bg_->Draw();
}

void TitleScene::Draw(void)
{
	DrawerIns.Add2DDraw(playerAnim_->GetImageHandle(), playerPos_.x, playerPos_.y,
		0.0, 4.0, true, 500, DrawType::Normal, playerGoForward_, false);

	if (isExiting_)
	{
		exitUI_->Draw();
	}
	else
	{
		ui_->Draw();
	}
}

void TitleScene::Initialize(void)
{
	camera_ = std::make_unique<Camera>();

	camera_->SetCameraPosition(400.0f, 300.0f, -800.0f);
	camera_->AddAngleDeg(0.0f, 0.0f, 0.0f);

	bg_ = std::make_unique<BackGround>();
	bg_->Load("Resource/BG/ForestBG.xml");
	bg_->SetCamera(camera_.get());

	auto&& [w, h] = Application::GetWindowSize();
	screenWidth_ = w;

	playerAnim_ = std::make_unique<Animation>("Resource/Animation/PlayerAnim.xml");

	ui_ = std::make_unique<TitleUI>("Resource/UI/TitleSceneUI.xml", this);
	ui_->SetVisible(false);

	exitUI_ = std::make_unique<ExitUI>("Resource/UI/ExitSceneUI.xml");
	exitUI_->SetVisible(false);

	AudioIns.LoadSound("Resource/Sound/BGM134-121031-honobonohanabana-wav.wav", "title_bgm");

	if (isBack_)
	{
		bgSpeed_ = 0.0f;
		playerPos_ = { screenWidth_ + 100.0f, h * 0.75f };
		playerSpeed_ = BackPlayerSpeed;
		update_ = &TitleScene::BackUpdate;
	}
	else
	{
		bgSpeed_ = SelectBGSpeed;
		playerPos_ = { 0.0f, h * 0.75f };
		playerSpeed_ = StartPlayerSpeed;
		update_ = &TitleScene::StartUpdate;
	}
}

void TitleScene::ChangeScene(const std::shared_ptr<Scene>& scene)
{
	mng_->ChangeScene(new Fade(mng_->GetCurrentScene(), scene, 0.5f, 0.5f));
}

void TitleScene::StartUpdate(float delta)
{
	camera_->AddCameraPosition(bgSpeed_ * delta * 1.5f, 0.0f, 0.0f);

	playerAnim_->Update(delta);

	bg_->Update(delta);

	playerPos_.x += playerSpeed_ * delta;

	if (playerPos_.x >= screenWidth_ * 0.5f)
	{
		playerPos_.x = static_cast<float>(screenWidth_ * 0.5f);
		ui_->SetVisible(true);
		bgmHandle_ = AudioIns.PlayLoop("title_bgm", 0.0f, 0.0f, XAUDIO2_LOOP_INFINITE, 
			1.0f, true, { SubmixHandleMng.GetHandle("bgm") });
		bgSpeed_ = SelectBGSpeed;
		update_ = &TitleScene::SelectUpdate;
	}
}

void TitleScene::BackUpdate(float delta)
{
	camera_->AddCameraPosition(bgSpeed_ * delta * 1.5f, 0.0f, 0.0f);

	playerAnim_->Update(delta);

	bg_->Update(delta);

	playerPos_.x += playerSpeed_ * delta;

	if (playerPos_.x <= screenWidth_ * 0.5f)
	{
		playerPos_.x = static_cast<float>(screenWidth_ * 0.5f);
		ui_->SetVisible(true);
		bgmHandle_ = AudioIns.PlayLoop("title_bgm", 0.0f, 0.0f, XAUDIO2_LOOP_INFINITE,
			1.0f, true, { SubmixHandleMng.GetHandle("bgm") });
		bgSpeed_ = SelectBGSpeed;
		playerGoForward_ = true;
		update_ = &TitleScene::SelectUpdate;
	}
}

void TitleScene::SelectUpdate(float delta)
{
	camera_->AddCameraPosition(bgSpeed_ * delta, 0.0f, 0.0f);

	playerAnim_->Update(delta);

	ui_->Update(delta);
	bg_->Update(delta);

	TitleUI::State state = ui_->GetState();

	auto&& s = ui_->GetState();

	if (s == TitleUI::State::EXIT)
	{
		exitUI_->SetVisible(true);
		update_ = &TitleScene::ExitSelectUpdate;
		time_ = 0.0f;
		isExiting_ = true;
		exitUI_->Reset();
		return;
	}

	if (s != TitleUI::State::NONE)
	{
		playerSpeed_ = (state == TitleUI::State::GO_SELECT) ? EndPlayerSpeed : BackPlayerSpeed;
		bgSpeed_ = (state == TitleUI::State::GO_SELECT) ? SelectBGSpeed : 0.0f;
		playerGoForward_ = (state == TitleUI::State::GO_SELECT);
		update_ = &TitleScene::EndUpdate;
		time_ = 0.0f;
	}
}

void TitleScene::ExitSelectUpdate(float delta)
{
	camera_->AddCameraPosition(bgSpeed_ * delta, 0.0f, 0.0f);

	playerAnim_->Update(delta);

	exitUI_->Update(delta);
	bg_->Update(delta);


	auto&& s = exitUI_->GetState();
	if (s != ExitUI::State::NONE)
	{
		switch (s)
		{
		case ExitUI::State::YES:
			Application::EndGame();
			break;
		case ExitUI::State::NO:
			exitUI_->SetVisible(false);
			update_ = &TitleScene::SelectUpdate;
			time_ = 0.0f;
			isExiting_ = false;
			ui_->Reset();
			break;
		default:
			break;
		}
	}
}

void TitleScene::EndUpdate(float delta)
{
	time_ += delta;

	camera_->AddCameraPosition(bgSpeed_ * delta * 1.5f, 0.0f, 0.0f);

	playerAnim_->Update(delta);

	playerPos_.x += playerSpeed_ * delta;
	AudioIns.SetVolume(bgmHandle_, std::clamp(1.0f - (time_ / EndDuration), 0.0f, 1.0f));

	bg_->Update(delta);
	if (time_ > EndDuration)
	{
		std::shared_ptr<Scene> next;
		switch (ui_->GetState())
		{
		case TitleUI::State::GO_SELECT:
			next = std::make_shared<StageSelectScene>(0);
			break;
		case TitleUI::State::GO_SETTING:
			next = std::make_shared<SettingScene>();
			break;
		default:
			break;
		}
		AudioIns.Stop(bgmHandle_);
		mng_->ChangeScene(new Fade(mng_->GetCurrentScene(), next, 1.0f, 1.0f));
	}
}
