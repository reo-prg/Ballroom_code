#include "UIIntegrator.h"
#include "../MyRapidXML_Helper.h"
#include "UIBase.h"
#include "UIImageRenderer.h"
#include "UITextRenderer.h"
#include "UIProgressBar.h"

UIIntegrator::UIIntegrator()
{
}

UIIntegrator::UIIntegrator(const std::string& filepath)
{
	InitializeFunc();
	LoadUIPlacement(filepath);
}

UIIntegrator::~UIIntegrator()
{
	delete file_;
	delete doc_;
}

UIBase* UIIntegrator::GetUIRef(const std::string& objectName)
{
	if (uiList_.find(objectName) == uiList_.end())
	{
		return nullptr;
	}
	return uiList_[objectName].get();
}

void UIIntegrator::AddUI(const std::string& objectName, UIBase* ui)
{
	uiList_.try_emplace(objectName, ui);
}

void UIIntegrator::Update(float delta)
{
	for (auto& u : uiList_)
	{
		u.second->Update(delta);
	}
}

void UIIntegrator::Draw(void)
{
	for (auto& u : uiList_)
	{
		u.second->Draw();
	}
}

void UIIntegrator::SetVisible(bool visibility)
{
	for (auto& [key, ui] : uiList_)
	{
		ui->SetVisible(visibility);
	}
}

void UIIntegrator::InitializeFunc(void)
{
	createfunc_.emplace("Image", &UIIntegrator::CreateImageUI);
	createfunc_.emplace("Text", &UIIntegrator::CreateTextUI);
	createfunc_.emplace("ProgressBar", &UIIntegrator::CreateBarUI);
	createTiledfunc_.emplace("Image", &UIIntegrator::CreateTiledImageUI);
	createTiledfunc_.emplace("Text", &UIIntegrator::CreateTiledTextUI);
	createTiledfunc_.emplace("ProgressBar", &UIIntegrator::CreateTiledBarUI);
}

void UIIntegrator::LoadUIPlacement(const std::string& filepath)
{
	file_ = new rapidxml::file<>(filepath.c_str());
	doc_ = new rapidxml::xml_document<>();
	doc_->parse<0>(file_->data());

	rapidxml::xml_node<>* root = doc_->first_node();

	// íPëÃUIÇÃçÏê¨
	for (auto mdnode = root->first_node("module"); mdnode != nullptr;
		mdnode = mdnode->next_sibling("module"))
	{
		CheckUIType(mdnode);
	}

	// ï¿ÇÒÇæUIÇÃçÏê¨
	for (auto tnode = root->first_node("tile"); tnode != nullptr;
		tnode = tnode->next_sibling("tile"))
	{
		CreateTiledUI(tnode);
	}
}

void UIIntegrator::CheckUIType(rapidxml::xml_node<>* node)
{
	std::string mtype = GetAttributeValue(node->first_attribute("type"));
	(this->*(createfunc_.at(mtype)))(node);
}

void UIIntegrator::CreateTiledUI(rapidxml::xml_node<>* node)
{
	for (auto mdnode = node->first_node("module"); mdnode != nullptr;
		mdnode = mdnode->next_sibling("module"))
	{
		CheckTiledUIType(node, mdnode);
	}
}

void UIIntegrator::CheckTiledUIType(rapidxml::xml_node<>* tilenode, rapidxml::xml_node<>* modulenode)
{
	int countx = atoi(GetAttributeValue(tilenode->first_attribute("countx")));
	int county = atoi(GetAttributeValue(tilenode->first_attribute("county")));
	float posx = static_cast<float>(atoi(GetAttributeValue(tilenode->first_attribute("posx"))));
	float posy = static_cast<float>(atoi(GetAttributeValue(tilenode->first_attribute("posy"))));
	float sizex = static_cast<float>(atoi(GetAttributeValue(tilenode->first_attribute("sizex"))));
	float sizey = static_cast<float>(atoi(GetAttributeValue(tilenode->first_attribute("sizey"))));


	Vector2<float> pos = { posx - sizex * 0.5f, posy - sizey * 0.5f };
	Vector2<float> diff = { sizex / static_cast<float>(countx - 1), sizey / static_cast<float>(county - 1) };

	// ïKóvÇ»å¬êîï™UIÇê∂ê¨
	for (int y = 0; y < county; y++)
	{
		for (int x = 0; x < countx; x++)
		{
			std::string mtype = GetAttributeValue(modulenode->first_attribute("type"));
			(this->*(createTiledfunc_.at(mtype)))
				(modulenode, pos.x + diff.x * x,pos.y + diff.y * y, y * countx + x);
		}
	}
}

void UIIntegrator::CreateImageUI(rapidxml::xml_node<>* node)
{
	rapidxml::xml_node<>* posnode = node->first_node("position");
	rapidxml::xml_node<>* sizenode = node->first_node("size");

	std::string texture = GetNodeValue(node->first_node("path"));
	float posx = static_cast<float>(atof(GetNodeValue(posnode->first_node("x"))));
	float posy = static_cast<float>(atof(GetNodeValue(posnode->first_node("y"))));
	float sizex = static_cast<float>(atof(GetNodeValue(sizenode->first_node("x"))));
	float sizey = static_cast<float>(atof(GetNodeValue(sizenode->first_node("y"))));
	float angle = static_cast<float>(atof(GetNodeValue(node->first_node("angle"))));
	int priority = atoi(GetNodeValue(node->first_node("priority")));

	// ê∂ê¨
	UIImageRenderer* imageUI = new UIImageRenderer({ posx, posy }, { sizex, sizey }, angle, texture, priority);

	// ÉäÉXÉgÇ…ìoò^
	std::string uiName = GetAttributeValue(node->first_attribute("name"));
	uiList_.try_emplace(uiName, imageUI);
}

void UIIntegrator::CreateTextUI(rapidxml::xml_node<>* node)
{
	rapidxml::xml_node<>* posnode = node->first_node("position");
	rapidxml::xml_node<>* colornode = node->first_node("color");

	float posx = static_cast<float>(atof(GetNodeValue(posnode->first_node("x"))));
	float posy = static_cast<float>(atof(GetNodeValue(posnode->first_node("y"))));
	int red = static_cast<int>(atof(GetNodeValue(colornode->first_node("red"))) * 255.0);
	int green = static_cast<int>(atof(GetNodeValue(colornode->first_node("green"))) * 255.0);
	int blue = static_cast<int>(atof(GetNodeValue(colornode->first_node("blue"))) * 255.0);
	int size = atoi(GetNodeValue(node->first_node("size")));
	int thick = atoi(GetNodeValue(node->first_node("thickness")));
	int priority = atoi(GetNodeValue(node->first_node("priority")));
	std::string font = GetNodeValue(node->first_node("font"));
	std::string text = GetNodeValue(node->first_node("text"));

	std::string uiname = GetAttributeValue(node->first_attribute("name"));

	UITextRenderer* textUI;

	// ÉtÉHÉìÉgÇ™éwíËÇ≥ÇÍÇƒÇ¢ÇÈÇ©
	if (node->first_node("fontpath") != nullptr)
	{
		std::string fontpath = GetNodeValue(node->first_node("fontpath"));
		textUI = new UITextRenderer({ posx, posy }, font, fontpath, size, thick, text, red, green, blue, uiname, priority);
	}
	else
	{
		textUI = new UITextRenderer({ posx, posy }, font, size, thick, text, red, green, blue, uiname, priority);
	}

	// ÉäÉXÉgÇ…ìoò^
	uiList_.emplace(uiname, textUI);
}

void UIIntegrator::CreateBarUI(rapidxml::xml_node<>* node)
{
	rapidxml::xml_node<>* posnode = node->first_node("position");

	float posx = static_cast<float>(atof(GetNodeValue(posnode->first_node("x"))));
	float posy = static_cast<float>(atof(GetNodeValue(posnode->first_node("y"))));
	int priority = atoi(GetNodeValue(node->first_node("priority")));
	std::string base = GetNodeValue(node->first_node("base"));
	std::string progress = GetNodeValue(node->first_node("progress"));
	std::string uiName = GetAttributeValue(node->first_attribute("name"));

	UIProgressBar* bar;

	// ñ⁄ê∑ÇËÇÃâÊëúÇ™Ç†ÇÈÇ©Ç«Ç§Ç©
	if (node->first_node("div") != nullptr)
	{
		std::string div = GetNodeValue(node->first_node("div"));

		bar = new UIProgressBar({ posx, posy }, base, progress, div, uiName, priority);
	}
	else
	{
		bar = new UIProgressBar({ posx, posy }, base, progress, uiName, priority);
	}
	
	// ÉäÉXÉgÇ…ìoò^
	uiList_.try_emplace(uiName, bar);
}

void UIIntegrator::CreateTiledImageUI(rapidxml::xml_node<>* node, float x, float y, int index)
{
	rapidxml::xml_node<>* posnode = node->first_node("position");
	rapidxml::xml_node<>* sizenode = node->first_node("size");

	std::string texture = GetNodeValue(node->first_node("path"));
	float posx = static_cast<float>(atof(GetNodeValue(posnode->first_node("x"))));
	float posy = static_cast<float>(atof(GetNodeValue(posnode->first_node("y"))));
	float sizex = static_cast<float>(atof(GetNodeValue(sizenode->first_node("x"))));
	float sizey = static_cast<float>(atof(GetNodeValue(sizenode->first_node("y"))));
	float angle = static_cast<float>(atof(GetNodeValue(node->first_node("angle"))));
	int priority = atoi(GetNodeValue(node->first_node("priority")));
	
	// ê∂ê¨
	UIImageRenderer* imageUI = new UIImageRenderer({ posx + x, posy + y }, { sizex, sizey }, angle, texture, priority);

	std::string uiname = GetAttributeValue(node->first_attribute("name"));
	uiname = uiname + std::to_string(index);
	
	// ÉäÉXÉgÇ…ìoò^
	uiList_.try_emplace(uiname, imageUI);
}

void UIIntegrator::CreateTiledTextUI(rapidxml::xml_node<>* node, float x, float y, int index)
{
	rapidxml::xml_node<>* posnode = node->first_node("position");
	rapidxml::xml_node<>* colornode = node->first_node("color");

	float posx = static_cast<float>(atof(GetNodeValue(posnode->first_node("x"))));
	float posy = static_cast<float>(atof(GetNodeValue(posnode->first_node("y"))));
	int red = static_cast<int>(atof(GetNodeValue(colornode->first_node("red"))) * 255.0);
	int green = static_cast<int>(atof(GetNodeValue(colornode->first_node("green"))) * 255.0);
	int blue = static_cast<int>(atof(GetNodeValue(colornode->first_node("blue"))) * 255.0);
	int size = atoi(GetNodeValue(node->first_node("size")));
	int thick = atoi(GetNodeValue(node->first_node("thickness")));
	int priority = atoi(GetNodeValue(node->first_node("priority")));
	std::string font = GetNodeValue(node->first_node("font"));
	std::string text = GetNodeValue(node->first_node("text"));

	std::string uiName = GetAttributeValue(node->first_attribute("name"));
	uiName = uiName + std::to_string(index);

	UITextRenderer* textUI;

	// ÉtÉHÉìÉgÇ™éwíËÇ≥ÇÍÇƒÇ¢ÇÈÇ©
	if (node->first_node("fontpath") != nullptr)
	{
		std::string fontpath = GetNodeValue(node->first_node("fontpath"));
		textUI = new UITextRenderer({ posx + x, posy + y }, font, fontpath, size, thick, text, red, green, blue, uiName, priority);
	}
	else
	{
		textUI = new UITextRenderer({ posx + x, posy + y }, font, size, thick, text, red, green, blue, uiName, priority);
	}

	// ÉäÉXÉgÇ…ìoò^
	uiList_.emplace(uiName, textUI);
}

void UIIntegrator::CreateTiledBarUI(rapidxml::xml_node<>* node, float x, float y, int index)
{
	rapidxml::xml_node<>* posnode = node->first_node("position");

	float posx = static_cast<float>(atof(GetNodeValue(posnode->first_node("x"))));
	float posy = static_cast<float>(atof(GetNodeValue(posnode->first_node("y"))));
	int priority = atoi(GetNodeValue(node->first_node("priority")));
	std::string base = GetNodeValue(node->first_node("base"));

	std::string progress = GetNodeValue(node->first_node("progress"));
	std::string uiName = GetAttributeValue(node->first_attribute("name"));
	uiName = uiName + std::to_string(index);

	UIProgressBar* bar;

	// ñ⁄ê∑ÇËÇÃâÊëúÇ™Ç†ÇÈÇ©Ç«Ç§Ç©
	if (node->first_node("div") != nullptr)
	{
		std::string div = GetNodeValue(node->first_node("div"));

		bar = new UIProgressBar({ posx + x, posy + y }, base, progress, div, uiName, priority);
	}
	else
	{
		bar = new UIProgressBar({ posx + x, posy + y }, base, progress, uiName, priority);
	}

	// ÉäÉXÉgÇ…ìoò^
	uiList_.try_emplace(uiName, bar);
}
