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
	/// string����wstring�ɕϊ�
	/// </summary>
	/// <param name="str">�ϊ�������������</param>
	/// <returns>�ϊ�����������</returns>
	std::wstring WstringToString(const std::string& str)
	{
		std::wstring ret;
		int val1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
		ret.resize(val1);
		int val2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &ret[0], val1);
		return ret;
	}

	/// <summary>
	/// �g���q�̎擾
	/// </summary>
	/// <param name="str">���̕�����</param>
	/// <returns>�g���q</returns>
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
	/// �����𒲂ׂ�
	/// </summary>
	/// <param name="value">���ׂ�l</param>
	/// <returns>+�Ȃ�+1 -�Ȃ�-1</returns>
	template<typename T>
	T sign(const T& value)
	{
		static_assert(std::is_arithmetic<T>::value);
		return (value < static_cast<T>(0)) - (value > static_cast<T>(0));
	}

	/// <summary>
	/// ���`���
	/// </summary>
	/// <param name="val1">�J�n�̒l</param>
	/// <param name="val2">�I���̒l</param>
	/// <param name="t">��Ԃ̒l</param>
	/// <returns>val1 + (val2 - val1) * t</returns>
	float Lerp(float val1, float val2, float t)
	{
		return val1 + (val2 - val1) * t;
	}
}
