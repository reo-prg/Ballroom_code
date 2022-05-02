#include "ExitUI.h"
#include "../UIBase.h"
#include "../UIImageRenderer.h"
#include "../UITextRenderer.h"
#include "../../Audio/AudioManager.h"
#include "../../Audio/SubmixHandleManager.h"
#include "../../Input/InputIntegrator.h"
#include "../../MyRapidXML_Helper.h"

ExitUI::ExitUI(const std::string& filepath)
	: UIIntegrator(filepath)
{
	Initialize();
}

ExitUI::~ExitUI()
{
}

void ExitUI::Update(float delta)
{
	UIIntegrator::Update(delta);

	time_ += delta;

	cursor_->SetVisible(static_cast<int>(time_ * 4.0f) % 2 == 0);

	MoveCursor();
	Select();
}

void ExitUI::Draw(void)
{
	UIIntegrator::Draw();
}

void ExitUI::Reset(void)
{
	time_ = 0.0f;
	cursorpos_ = 0;
	state_ = State::NONE;

	for (int i = 0; i < select_.size(); i++)
	{
		select_[i]->SetColor(cursorpos_ == i ? selectColor : idleColor);
	}
}

void ExitUI::Initialize(void)
{
	time_ = 0.0f;
	cursorpos_ = 0;

	rapidxml::xml_node<>* add = doc_->first_node();
	rapidxml::xml_node<>* arr = add->first_node("additional")->first_node("cursorarray");

	for (auto e = arr->first_node("elem"); e != nullptr; e = e->next_sibling("elem"))
	{
		UITextRenderer* txt = GetUIRefWithDownCast<UITextRenderer>(GetNodeValue(e));
		select_.push_back(txt);
	}

	cursor_ = GetUIRefWithDownCast<UIImageRenderer>("cursor");
	if (!cursor_)
	{
		return;
	}

	select_[cursorpos_]->SetColor(selectColor);
	cursor_->SetPositionY(select_[cursorpos_]->GetCenterPositionY());
}

void ExitUI::MoveCursor(void)
{
	if (InputIns.Triggered("up"))
	{
		select_[cursorpos_]->SetColor(idleColor);

		int sz = static_cast<int>(select_.size());
		cursorpos_ = (--cursorpos_ + sz) % sz;

		select_[cursorpos_]->SetColor(selectColor);

		auto&& tPos = select_[cursorpos_]->GetPosition();
		cursor_->SetPositionY(select_[cursorpos_]->GetCenterPositionY());

		AudioIns.PlayTemporary("select", 0.6f, { SubmixHandleMng.GetHandle("se") });
	}
	if (InputIns.Triggered("down"))
	{
		select_[cursorpos_]->SetColor(idleColor);

		cursorpos_ = ++cursorpos_ % select_.size();

		select_[cursorpos_]->SetColor(selectColor);
		cursor_->SetPositionY(select_[cursorpos_]->GetCenterPositionY());
		AudioIns.PlayTemporary("select", 0.6f, { SubmixHandleMng.GetHandle("se") });
	}
}

void ExitUI::Select(void)
{
	if (InputIns.Triggered("ok"))
	{
		AudioIns.PlayTemporary("button", 0.6f, { SubmixHandleMng.GetHandle("se") });
		MoveToNextScene();
	}
}

void ExitUI::MoveToNextScene(void)
{
	switch (static_cast<ExitUI::SelectElement>(cursorpos_))
	{
	case ExitUI::SelectElement::Yes:
		/*scene_->ChangeScene(std::make_shared<StageSelectScene>());*/
		state_ = State::YES;
		break;
	case ExitUI::SelectElement::No:
		//scene_->ChangeScene(std::make_shared<SettingScene>());
		state_ = State::NO;
		break;
	default:
		break;
	}
}
