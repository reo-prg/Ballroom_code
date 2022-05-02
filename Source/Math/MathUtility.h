#pragma once
#include <algorithm>
#include <type_traits>

namespace
{
	constexpr float Math_PI = 3.14159265f;

	constexpr float DegreeToRadian(float angle)
	{
		return angle * Math_PI / 180.0f;
	}

	constexpr float RadianToDegree(float angle)
	{
		return angle * 180.0f / Math_PI;
	}

	template <typename T>
	bool IsPositive(T value)
	{
		static_assert(std::is_arithmetic<T>::value);
		return value >= T(0);
	}

	template<typename T>
	constexpr T CPow(T base, T exp)
	{
		return exp <= 0 ? 1 :
			exp == 1 ? base :
			base * CPow(base, exp - 1);
	}

	float Saturate(float val)
	{
		return 1.0f < val ? 1.0f :
			0.0f > val ? 0.0f : val;
	}

	float SquareDistance(float x1, float y1, float x2, float y2)
	{
		return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	}
}
