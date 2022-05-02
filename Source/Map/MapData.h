#pragma once
#include <string>
#include "../../RapidXML/rapidxml.hpp"
#include "../../RapidXML/rapidxml_utils.hpp"
#include "../MyRapidXML_Helper.h"

struct MapData
{
	void Load(rapidxml::xml_node<>* node)
	{
		modelPath_ = GetAttributeValue(node->first_attribute("model"));
		collisionPath_ = GetAttributeValue(node->first_attribute("collision"));
		actorPath_ = GetAttributeValue(node->first_attribute("actor"));
		bgPath_ = GetAttributeValue(node->first_attribute("bg"));
		bgmPath_ = GetAttributeValue(node->first_attribute("music"));
	}

	std::string modelPath_;
	std::string collisionPath_;
	std::string actorPath_;
	std::string bgPath_;
	std::string bgmPath_;
	int stage_;
};
