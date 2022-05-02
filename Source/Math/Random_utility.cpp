#include "Random_utility.h"

rand_util::RandomGen gen;

rand_util::RandomGen::RandomGen()
{
	std::random_device rd;
	mt_ = std::mt19937(rd());
}

std::mt19937& rand_util::GetSeed(void)
{
	return gen.mt_;
}
