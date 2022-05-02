#pragma once
#include <random>

namespace rand_util
{
	struct RandomGen
	{
		RandomGen();
		std::mt19937 mt_;
	};

	std::mt19937& GetSeed(void);
}
