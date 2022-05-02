#include "StageSelectUI.h"
#include <algorithm>
#include "../../Audio/AudioManager.h"
#include "../../Audio/SubmixHandleManager.h"
#include "../../Scene/StageSelectScene.h"
#include "../../Scene/TitleScene.h"
#include "../../Scene/MainScene.h"
#include "../../MyRapidXML_Helper.h"
#include "../UIImageRenderer.h"
#include "../UITextRenderer.h"
#include "../../Input/InputIntegrator.h"
#include "../../Graphic/ImageHandleManager.h"
#include "../../Map/MapData.h"


namespace
{
	const std::string StageTablePath = "Resource/Map/StagePathTable.xml";

	constexpr float NotSelectScale = 1.0f;
	constexpr float SelectScale = 1.25f;
	constexpr float SelectScaleSpeed = 3.0f;
}

StageSelectUI::StageSelectUI(const std::string& filepath, StageSelectScene* scene)
	: UIIntegrator(filepath), scene_(scene)
{
	Initialize();
}

StageSelectUI::~StageSelectUI()
{
	stagedoc_.clear();
}

void StageSelectUI::Update(float delta)
{
	UIIntegrator::Update(delta);
	MoveCursor();
	ButtonScaleUpdate(delta);
	Select();
}

void StageSelectUI::Draw(void)
{
	UIIntegrator::Draw();
}

void StageSelectUI::SetCursorPos(int pos)
{
	cursorpos_ = pos;
	ButtonUpdate(0, cursorpos_);
}

void StageSelectUI::Initialize(void)
{
	MakeButtons();
	ImageMngIns.Load("selected", "Resource/Texture/General/fsy002/png/button_empty_01.png");
	ImageMngIns.Load("notselected", "Resource/Texture/General/fsy002/png/button_empty_02.png");

	selectGHandle_ = ImageMngIns.Get("selected")[0];
	notSelectGHandle_ = ImageMngIns.Get("notselected")[0];

	select_[cursorpos_]->SetTexture(selectGHandle_);
}

void StageSelectUI::MakeButtons(void)
{
	rapidxml::xml_node<>* tilenode = doc_->first_node()->first_node("tile");
	count_x = atoi(GetAttributeValue(tilenode->first_attribute("countx")));
	count_y = atoi(GetAttributeValue(tilenode->first_attribute("county")));
	float posx = static_cast<float>(atoi(GetAttributeValue(tilenode->first_attribute("posx"))));
	float posy = static_cast<float>(atoi(GetAttributeValue(tilenode->first_attribute("posy"))));
	float sizex = static_cast<float>(atoi(GetAttributeValue(tilenode->first_attribute("sizex"))));
	float sizey = static_cast<float>(atoi(GetAttributeValue(tilenode->first_attribute("sizey"))));


	Vector2<float> pos = { posx - sizex * 0.5f, posy - sizey * 0.5f };
	Vector2<float> diff = { sizex / static_cast<float>(count_x - 1), sizey / static_cast<float>(count_y - 1) };

	select_.resize(count_x * count_y);
	buttonScale_.resize(count_x * count_y);
	for (int y = 0; y < count_y; y++)
	{
		for (int x = 0; x < count_x; x++)
		{
			int idx = y * count_x + x;
			std::string id = "select_number" + std::to_string(idx);
			auto&& textref = GetUIRefWithDownCast<UITextRenderer>(id);
			textref->SetText(std::to_string(idx + 1));
			
			id = "select_button" + std::to_string(idx);
			auto&& imgref = GetUIRefWithDownCast<UIImageRenderer>(id);
			select_[idx] = imgref;

			buttonScale_[idx] = 1.0f;
		}
	}
}

void StageSelectUI::MoveCursor(void)
{
	int nextcursor = cursorpos_;
	if (InputIns.Triggered("up"))
	{
		nextcursor = (nextcursor + count_x) % static_cast<int>(select_.size());
	}
	if (InputIns.Triggered("down"))
	{
		nextcursor = (nextcursor - count_x + static_cast<int>(select_.size())) % static_cast<int>(select_.size());
	}
	if (InputIns.Triggered("left"))
	{
		nextcursor = (--nextcursor + static_cast<int>(select_.size())) % static_cast<int>(select_.size());
	}
	if (InputIns.Triggered("right"))
	{
		nextcursor = ++nextcursor % static_cast<int>(select_.size());
	}

	if (cursorpos_ != nextcursor)
	{
		ButtonUpdate(cursorpos_, nextcursor);
		cursorpos_ = nextcursor;
		AudioIns.PlayTemporary("select", 0.6f, { SubmixHandleMng.GetHandle("se") });
	}
}

void StageSelectUI::Select(void)
{
	if (InputIns.Triggered("ok"))
	{
		AudioIns.PlayTemporary("button", 0.6f, { SubmixHandleMng.GetHandle("se") });
		state_ = State::GO_GAME;
	}
	if (InputIns.Triggered("back"))
	{
		AudioIns.PlayTemporary("button", 0.6f, { SubmixHandleMng.GetHandle("se") });
		state_ = State::GO_TITLE;
	}
}

void StageSelectUI::ButtonUpdate(int prev, int next)
{
	if (prev == next) { return; }
	select_[prev]->SetTexture(notSelectGHandle_);
	select_[next]->SetTexture(selectGHandle_);
}

void StageSelectUI::ButtonScaleUpdate(float delta)
{
	for (int i = 0; i < buttonScale_.size(); i++)
	{
		if (i == cursorpos_)
		{
			buttonScale_[i] += delta * SelectScaleSpeed;
		}
		else
		{
			buttonScale_[i] -= delta * SelectScaleSpeed;
		}
		buttonScale_[i] = std::clamp(buttonScale_[i], NotSelectScale, SelectScale);
		select_[i]->SetScale(buttonScale_[i]);
	}
}
