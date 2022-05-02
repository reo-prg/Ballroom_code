#pragma once
#include <string>
#include <Windows.h>

struct Size
{
	int x;
	int y;
};
namespace
{
	/// <summary>
	/// stringからwstringに変換
	/// </summary>
	/// <param name="str">変換したい文字列</param>
	/// <returns>変換した文字列</returns>
	std::wstring WstringToString(const std::string& str)
	{
		std::wstring ret;
		int val1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
		ret.resize(val1);
		int val2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &ret[0], val1);
		return ret;
	}

	/// <summary>
	/// 拡張子の取得
	/// </summary>
	/// <param name="str">元の文字列</param>
	/// <returns>拡張子</returns>
	std::string GetExtension(const std::string& str)
	{
		size_t idx = str.rfind(".");
		if (idx == std::string::npos)
		{
			return std::string();
		}

		return str.substr(idx + 1);
	}

	unsigned int AlignTo(unsigned int value, unsigned int align)
	{
		return value + (align - (value % align)) % align;
	}

	/// <summary>
	/// 符号を調べる
	/// </summary>
	/// <param name="value">調べる値</param>
	/// <returns>+なら+1 -なら-1</returns>
	template<typename T>
	T sign(const T& value)
	{
		static_assert(std::is_arithmetic<T>::value);
		return (value < static_cast<T>(0)) - (value > static_cast<T>(0));
	}

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="val1">開始の値</param>
	/// <param name="val2">終了の値</param>
	/// <param name="t">補間の値</param>
	/// <returns>val1 + (val2 - val1) * t</returns>
	float Lerp(float val1, float val2, float t)
	{
		return val1 + (val2 - val1) * t;
	}
}
