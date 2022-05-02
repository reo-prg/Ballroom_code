#include "AnimationManager.h"
#include "ImageHandleManager.h"
#include "../MyRapidXML_Helper.h"
#include <EffekseerForDXLib.h>

AnimationManager* AnimationManager::instance_ = nullptr;


void AnimationManager::LoadAnimation(std::string_view filename)
{
	if (animations_.contains(std::string(filename.data())))
	{
		return;
	}

	rapidxml::file<> file = filename.data();
	rapidxml::xml_document<>* rawData_ = new rapidxml::xml_document<>();
	rawData_->parse<0>(file.data());

	rapidxml::xml_node<>* infoNode = rawData_->first_node("info");
	std::string mode = GetAttributeValue(infoNode->first_attribute("mode"));

	if (mode == "int")
	{
		rapidxml::xml_node<>* imageNode = infoNode->first_node("image");

		std::string imagePath = GetAttributeValue(imageNode->first_attribute("path"));
		int divX = atoi(GetAttributeValue(imageNode->first_attribute("divx")));
		int divY = atoi(GetAttributeValue(imageNode->first_attribute("divy")));
		int sizeX = atoi(GetAttributeValue(imageNode->first_attribute("sizex")));
		int sizeY = atoi(GetAttributeValue(imageNode->first_attribute("sizey")));

		ImageMngIns.Load(imagePath, imagePath, divX, divY, sizeX, sizeY);

		animations_[filename.data()].imagePath_ = imagePath;

		for (auto animNode = imageNode->first_node("animation");
			animNode != nullptr; animNode = animNode->next_sibling())
		{
			std::string name = GetAttributeValue(animNode->first_attribute("name"));
			int loop = atoi(GetAttributeValue(animNode->first_attribute("loop")));
			std::vector<std::pair<int, double>> datas;
			double time = 0.0;

			for (auto dataNode = animNode->first_node("data");
				dataNode != nullptr; dataNode = dataNode->next_sibling())
			{
				int id = atoi(GetAttributeValue(dataNode->first_attribute("id")));
				time += atof(GetAttributeValue(dataNode->first_attribute("time")));

				datas.emplace_back(std::pair<int, double>{ id, time });
			}

			animations_[filename.data()].animation_[name] = { std::move(datas), loop };
		}
	}
	else if (mode == "div")
	{
		rapidxml::xml_node<>* imageNode = infoNode->first_node("image");
		std::string dirPath = GetAttributeValue(imageNode->first_attribute("rootpath"));

		std::string imagePath = std::string(filename);
		animations_[filename.data()].imagePath_ = imagePath;
		std::vector<int> handles;
		int id = 0;

		for (auto animNode = imageNode->first_node("animation");
			animNode != nullptr; animNode = animNode->next_sibling())
		{
			std::string name = GetAttributeValue(animNode->first_attribute("name"));
			int loop = atoi(GetAttributeValue(animNode->first_attribute("loop")));
			std::vector<std::pair<int, double>> datas;
			double time = 0.0;

			for (auto dataNode = animNode->first_node("data");
				dataNode != nullptr; dataNode = dataNode->next_sibling())
			{
				std::string fid = GetAttributeValue(dataNode->first_attribute("id"));
				time += atof(GetAttributeValue(dataNode->first_attribute("time")));

				datas.emplace_back(std::pair<int, double>{ id, time });

				handles.push_back(LoadGraph((dirPath + name + fid).c_str()));

				id++;
			}

			animations_[filename.data()].animation_[name] = { std::move(datas), loop };
		}

		ImageMngIns.Register(imagePath, handles);
	}

	rawData_->clear();
	delete rawData_;
}

std::string& AnimationManager::GetImagePath(std::string_view filename)
{
	return animations_[std::string(filename)].imagePath_;
}

std::unordered_map<std::string, AnimData>& AnimationManager::GetAnimations(const std::string& filename)
{
	return animations_[std::string(filename)].animation_;
}

AnimData& AnimationManager::GetAnimation(std::string_view filename, std::string_view state)
{
	return animations_[std::string(filename)].animation_[std::string(state)];
}
