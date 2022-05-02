#pragma once
#include "../RapidXML/rapidxml.hpp"
#include "../RapidXML/rapidxml_utils.hpp"

namespace
{
	const char* GetAttributeValue(rapidxml::xml_attribute<>* attr)
	{
		if (attr->value_size())
		{
			return attr->value();
		}
		return "Invalid";
	}

	const char* GetNodeValue(rapidxml::xml_node<>* node)
	{
		if (node->value_size())
		{
			return node->value();
		}
		return "Invalid";
	}

	const char* GetNodeName(rapidxml::xml_node<>* node)
	{
		if (node->name_size())
		{
			return node->name();
		}
		return "Invalid";
	}
}
