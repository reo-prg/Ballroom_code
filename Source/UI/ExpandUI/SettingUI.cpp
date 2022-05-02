#include "SettingUI.h"
#include <algorithm>
#include "../UIBase.h"
#include "../UIImageRenderer.h"
#include "../UITextRenderer.h"
#include "../UIProgressBar.h"
#include "../../MyRapidXML_Helper.h"
#include "../../Audio/AudioManager.h"
#include "../../Audio/SubmixHandleManager.h"
#include "../../Input/InputIntegrator.h"

namespace
{
	std::array<std::string, 3> submixHandleList =
	{
		"master", "bgm", "se"
	};

	std::array<Vector2<float>, 3> submixVolumeList =
	{
		Vector2<float>{0.0f, 1.0f},
		Vector2<float>{0.01f, 0.3f},
		Vector2<float>{0.01f, 0.3f},
	};

	float volumeMin = 0.01f;
	float volumeMax = 0.3f;
}

SettingUI::SettingUI(const std::string& filepath, SettingScene* scene)
	: UIIntegrator(filepath), scene_(scene)
{
	Initialize();
}

SettingUI::~SettingUI()
{
}

void SettingUI::Update(float delta)
{
	UIIntegrator::Update(delta);

	time_ += delta;

	cursor_->SetVisible(static_cast<int>(time_ * 4.0f) % 2 == 0);

	MoveCursor();
	MoveBar(delta);
	Select();
}

void SettingUI::Draw(void)
{
	UIIntegrator::Draw();
}

void SettingUI::Initialize(void)
{
	time_ = 0.0f;
	cursorpos_ = 0;

	rapidxml::xml_node<>* add = doc_->first_node();
	rapidxml::xml_node<>* arr = add->first_node("additional")->first_node("cursorarray");

	for (auto e = arr->first_node("elem"); e != nullptr; e = e->next_sibling("elem"))
	{
		UIProgressBar* txt = GetUIRefWithDownCast<UIProgressBar>(GetNodeValue(e));
		select_.push_back(txt);
	}

	auto&& inst = AudioManager::GetInstance();
	for (int i = 0; i < select_.size(); i++)
	{
		float v = inst.GetVolume(SubmixHandleMng.GetHandle(submixHandleList[i]));
		select_[i]->SetProgress
		((v - submixVolumeList[i].x) / (submixVolumeList[i].y - submixVolumeList[i].x));
	}

	cursor_ = GetUIRefWithDownCast<UIImageRenderer>("cursor");
	if (!cursor_)
	{
		return;
	}
	cursor_->SetPositionY(select_[cursorpos_]->GetCenterPositionY());
}

void SettingUI::MoveCursor(void)
{
	if (InputIns.Triggered("up"))
	{
		int sz = static_cast<int>(select_.size());
		cursorpos_ = (--cursorpos_ + sz) % sz;

		auto&& tPos = select_[cursorpos_]->GetPosition();
		cursor_->SetPositionY(select_[cursorpos_]->GetCenterPositionY());

		AudioIns.PlayTemporary("select", 0.6f, { SubmixHandleMng.GetHandle("se") });
		time_ = 0.0f;
	}
	if (InputIns.Triggered("down"))
	{
		cursorpos_ = ++cursorpos_ % select_.size();

		cursor_->SetPositionY(select_[cursorpos_]->GetCenterPositionY());
		AudioIns.PlayTemporary("select", 0.6f, { SubmixHandleMng.GetHandle("se") });
		time_ = 0.0f;
	}
}

void SettingUI::MoveBar(float delta)
{
	float m = 0.0f;
	if (InputIns.Pressed("left"))
	{
		m -= delta * 0.5f;
	}
	if (InputIns.Pressed("right"))
	{
		m += delta * 0.5f;
	}

	select_[cursorpos_]->AddProgress(m);
	
	AudioIns.SetVolume(SubmixHandleMng.GetHandle(submixHandleList[cursorpos_]),
		std::lerp(submixVolumeList[cursorpos_].x, submixVolumeList[cursorpos_].y, select_[cursorpos_]->GetProgress()));
}

void SettingUI::Select(void)
{
	if (InputIns.Triggered("ok") || InputIns.Triggered("back"))
	{
		AudioIns.PlayTemporary("button", 0.6f, { SubmixHandleMng.GetHandle("se") });
		MoveToNextScene();
	}
}

void SettingUI::MoveToNextScene(void)
{
	state_ = State::BACK;
}
