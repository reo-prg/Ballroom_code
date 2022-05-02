#pragma once
#include <array>
#include <string>

enum class ItemType
{
	Heal,
	Buff,
	Max
};

const std::array<std::string, static_cast<size_t>(ItemType::Max)> ItemNames =
{
	"heal",
	"buff"
};
