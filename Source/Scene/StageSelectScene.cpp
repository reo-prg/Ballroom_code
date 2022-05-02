#include "StageSelectScene.h"
#include "../../RapidXML/rapidxml.hpp"
#include "../../RapidXML/rapidxml_utils.hpp"
#include "../MyRapidXML_Helper.h"
#include "SceneManager.h"
#include "Transition/Fade.h"
#include "../Audio/AudioManager.h"
#include "../Audio/SubmixHandleManager.h"
#include "../UI/ExpandUI/StageSelectUI.h"
#include "../Graphic/Camera.h"
#include "../Map/BackGround.h"
#include "../Map/MapData.h"
#include "../Graphic/Animation.h"
#include "../Graphic/Drawer.h"
#include "../Application.h"
#include "MainScene.h"
#include "TitleScene.h"

namespace
{
	constexpr float SelectBGSpeed = 300.0f;

	constexpr float StartPlayerSpeed = 300.0f;
	constexpr float BackPlayerSpeed = -300.0f;
	constexpr float EndPlayerSpeed = 700.0f;
	constexpr float EndDuration = 1.5f;

	const std::string StageTablePath = "Resource/Map/StagePathTable.xml";
}

StageSelectScene::StageSelectScene(int stage)
	: stage_(stage)
{
}

void StageSelectScene::Update(float delta)
{
	(this->*update_)(delta);
}

void StageSelectScene::BeforeDraw(void)
{
	bg_->Draw();
}

void StageSelectScene::Draw(void)
{
	DrawerIns.Add2DDraw(playerAnim_->GetImageHandle(), playerPos_.x, playerPos_.y,
		0.0, 4.0, true, 500, DrawType::Normal, playerGoForward_, false);

	ui_->Draw();
}

void StageSelectScene::Initialize(void)
{

	camera_ = std::make_unique<Camera>();

	camera_->SetCameraPosition(400.0f, 300.0f, -800.0f);
	camera_->AddAngleDeg(0.0f, 0.0f, 0.0f);

	bg_ = std::make_unique<BackGround>();
	bg_->Load("Resource/BG/ForestBG.xml");
	bg_->SetCamera(camera_.get());

	auto&& [w, h] = Application::GetWindowSize();
	playerPos_ = { -100.0f, h * 0.75f };
	screenWidth_ = w;

	playerAnim_ = std::make_unique<Animation>("Resource/Animation/PlayerAnim.xml");

	ui_ = std::make_unique<StageSelectUI>("Resource/UI/SelectSceneUI.xml", this);
	ui_->SetCursorPos(stage_);

	bgSpeed_ = SelectBGSpeed;
	playerGoForward_ = true;

	AudioIns.LoadSound("Resource/Sound/BGM115-110921-youseihouse-wav.wav", "select_bgm");
	bgmHandle_ = AudioIns.PlayLoop("select_bgm", 0.0f, 0.0f, XAUDIO2_LOOP_INFINITE, 
		0.5f, true, { SubmixHandleMng.GetHandle("bgm") });

	update_ = &StageSelectScene::StartUpdate;
}

void StageSelectScene::ChangeScene(const std::shared_ptr<Scene>& scene)
{
	mng_->ChangeScene(new Fade(mng_->GetCurrentScene(), scene, 0.5f, 0.5f));
}

void StageSelectScene::StartUpdate(float delta)
{
	camera_->AddCameraPosition(bgSpeed_ * delta * 1.5f, 0.0f, 0.0f);

	playerAnim_->Update(delta);

	bg_->Update(delta);

	playerPos_.x += StartPlayerSpeed * delta;

	AudioIns.SetVolume(bgmHandle_, std::clamp(0.5f * (playerPos_.x / (screenWidth_ * 0.25f)) + 0.5f, 0.0f, 1.0f));

	if (playerPos_.x >= screenWidth_ * 0.25f)
	{
		playerPos_.x = static_cast<float>(screenWidth_ * 0.25f);
		ui_->SetVisible(true);
		AudioIns.SetVolume(bgmHandle_, 1.0f);
		update_ = &StageSelectScene::SelectUpdate;
	}
}

void StageSelectScene::SelectUpdate(float delta)
{
	camera_->AddCameraPosition(bgSpeed_ * delta, 0.0f, 0.0f);

	playerAnim_->Update(delta);

	ui_->Update(delta);
	bg_->Update(delta);

	StageSelectUI::State state = ui_->GetState();
	if (state != StageSelectUI::State::NONE)
	{
		playerSpeed_ = (state == StageSelectUI::State::GO_GAME) ? EndPlayerSpeed : BackPlayerSpeed;
		bgSpeed_ = (state == StageSelectUI::State::GO_GAME) ? SelectBGSpeed : 0.0f;
		playerGoForward_ = (state == StageSelectUI::State::GO_GAME);
		update_ = &StageSelectScene::EndUpdate;
		time_ = 0.0f;
	}
}

void StageSelectScene::EndUpdate(float delta)
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
		MapData* mData;
		switch (ui_->GetState())
		{
		case StageSelectUI::State::GO_TITLE:
			next = std::make_shared<TitleScene>(true);
			break;
		case StageSelectUI::State::GO_GAME:
			mData = CreateMapData(ui_->GetCursorPos());
			next = std::make_shared<MainScene>(mData);
			break;
		default:
			break;
		}
		AudioIns.Stop(bgmHandle_);
		mng_->ChangeScene(new Fade(mng_->GetCurrentScene(), next, 1.0f, 1.0f));
	}
}

MapData* StageSelectScene::CreateMapData(int mapId)
{
	rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
	rapidxml::file<> stagefile = StageTablePath.c_str();
	doc->parse<0>(stagefile.data());

	rapidxml::xml_node<>* root = doc->first_node();
	int i = 0;
	MapData* mData = nullptr;

	for (auto stage = root->first_node("stage");
		stage != nullptr; stage = stage->next_sibling(), i++)
	{
		if (i != mapId) { continue; }

		mData = new MapData();
		mData->Load(stage);
		mData->stage_ = i;

		break;
	}
	return mData;
}
