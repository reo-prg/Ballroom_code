#include "BackGround.h"
#include "../Application.h"
#include "../../RapidXML/rapidxml.hpp"
#include "../../RapidXML/rapidxml_utils.hpp"
#include "../MyRapidXML_Helper.h"
#include "../Graphic/ImageHandleManager.h"
#include "../Graphic/Camera.h"
#include "../Graphic/Drawer.h"

BackGround::BackGround()
{
	auto [w, h] = Application::GetWindowSize();
	screenWidth_ = w;
	screenHeight_ = h;
}

BackGround::~BackGround()
{
}

void BackGround::Load(const std::string& filepath)
{
	rapidxml::file<> file = filepath.data();
	rapidxml::xml_document<>* rawData_ = new rapidxml::xml_document<>();
	rawData_->parse<0>(file.data());

	rapidxml::xml_node<>* rootNode = rawData_->first_node("root");

	for (auto layerNode = rootNode->first_node("layer");
		layerNode != nullptr; layerNode = layerNode->next_sibling())
	{
		std::string name = GetAttributeValue(layerNode->first_attribute("image"));
		float speed = static_cast<float>(atof(GetAttributeValue(layerNode->first_attribute("speed"))));

		ImageMngIns.Load(name);
		int h = ImageMngIns.Get(name)[0];

		int x, y;
		GetGraphSize(h, &x, &y);

		BGElem elem = {};
		elem.gHandle_ = h;
		elem.speed_ = speed;
		elem.offset_ = 0.0f;
		elem.scale_ = static_cast<float>(screenHeight_) / static_cast<float>(y);
		elem.width_ = static_cast<float>(x) * elem.scale_;

		elem_.push_back(std::move(elem));
	}
}

void BackGround::Update(float delta)
{

}

void BackGround::Draw(void)
{
	int cPos = 0;
	int y = screenHeight_ / 2;
	if (camera_)
	{
		cPos = -static_cast<int>(camera_->GetCameraPosition().x);
	}
	
	int count = 0;
	for (auto& e : elem_)
	{
		int os = static_cast<int>(fmodf(static_cast<float>(cPos) * e.speed_, e.width_));
		os = os >= 0 ? os : static_cast<int>(e.width_) + os;
		for (int x = -static_cast<int>(e.width_) + os; x < screenWidth_ + e.width_; x += e.width_)
		{
			DrawerIns.Add2DDraw(e.gHandle_, static_cast<float>(x), static_cast<float>(y), 0.0, e.scale_, true, 50 + count, DrawType::BG);
		}
		count++;
	}
}

void BackGround::SetCamera(Camera* camera)
{
	camera_ = camera;
}

void BackGround::Initialize(void)
{
}
