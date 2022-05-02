#pragma once
#include "../VectorInterface.h"

#undef min
#undef max

template<typename T>
Vector2<T>::~Vector2()
{
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& value)const
{
	return Vector2<T>(x + value.x, y + value.y);
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const T& value)const
{
	return Vector2<T>(x + value, y + value);
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& value)const noexcept
{
	if (std::is_arithmetic<T>::value == true)
	{
		return Vector2<T>(x - value.x, y - value.y);
	}
	else
	{
		return *this;
	}
}

template<typename T>
Vector2<T> Vector2<T>::operator*(const Vector2<T>& value)const noexcept
{
	if (std::is_arithmetic<T>::value == true)
	{
		return Vector2<T>(x * value.x, y * value.y);
	}
	else
	{
		return *this;
	}
}

template<typename T>
Vector2<T> Vector2<T>::operator*(float value)const noexcept
{
	if (std::is_arithmetic<T>::value == true)
	{
		return Vector2<T>(x * value, y * value);
	}
	else
	{
		return *this;
	}
}

template<typename T>
Vector2<T> Vector2<T>::operator/(const Vector2<T>& value)const
{
	if (std::is_arithmetic<T>::value == true)
	{
		if (value.x != static_cast<T>(0) && value.y != static_cast<T>(0))
		{
			return Vector2<T>(x / value.x, y / value.y);
		}
		else
		{
			return *this;
		}
	}
	else
	{
		return *this;
	}
}

template<typename T>
Vector2<T> Vector2<T>::operator/(float value)const
{
	if (std::is_arithmetic<T>::value == true)
	{
		if (value != 0.0f)
		{
			return Vector2<T>(x / value, y / value);
		}
		else
		{
			return *this;
		}
	}
	else
	{
		return *this;
	}
}

template<typename T>
Vector2<T> Vector2<T>::operator%(float value)
{
	if (std::is_arithmetic<T>::value == true)
	{
		return Vector2<T>(fmodf(static_cast<float>(x), value), fmodf(static_cast<float>(y), value));
	}
	else
	{
		return *this;
	}
}

template<typename T>
Vector2<T> Vector2<T>::operator%(int value)
{
	if (std::is_arithmetic<T>::value == true)
	{
		return Vector2<T>(static_cast<T>(static_cast<int>(x) % value),
			static_cast<T>(static_cast<int>(x) % value));
	}
	else
	{
		return *this;
	}
}

template<typename T>
void Vector2<T>::operator+= (const Vector2<T>& value)
{
	x = x + value.x;
	y = y + value.y;
}

template<typename T>
void Vector2<T>::operator+=(const T& value)
{
	x = x + value;
	y = y + value;
}

template<typename T>
void Vector2<T>::operator-=(const Vector2<T>& value)noexcept
{
	if (std::is_arithmetic<T>::value == true)
	{
		x = x - value.x;
		y = y - value.y;
	}
}

template<typename T>
void Vector2<T>::operator*=(const Vector2<T>& value)noexcept
{
	if (std::is_arithmetic<T>::value == true)
	{
		x = x * value.x;
		y = y * value.y;
	}
}

template<typename T>
void Vector2<T>::operator*=(float value)noexcept
{
	if (std::is_arithmetic<T>::value == true)
	{
		x = x * value;
		y = y * value;
	}
}

template<typename T>
void Vector2<T>::operator/=(const Vector2<T>& value)
{
	if (std::is_arithmetic<T>::value == true && value != 0.0f)
	{
		x = x / value.x;
		y = y / value.y;
	}
}

template<typename T>
void Vector2<T>::operator/=(float value)
{
	if (std::is_arithmetic<T>::value == true)
	{
		if (value != 0.0f)
		{
			x = x / value;
			y = y / value;
		}
		else
		{
			x = std::numeric_limits<T>::max();
			y = std::numeric_limits<T>::max();
		}
	}
}

template<typename T>
void Vector2<T>::operator%= (const Vector2<T>& value)
{
	if (std::is_arithmetic<T>::value == true)
	{
		if (static_cast<float>(value.x) != 0.0f)
		{
			x = x % value.x;
		}
		if (static_cast<float>(value.y) != 0.0f)
		{
			y = y % value.y;
		}
	}
}

template<typename T>
void Vector2<T>::operator%= (float value)
{
	if (std::is_arithmetic<T>::value == true && value != 0.0f)
	{
		x = fmodf(static_cast<float>(x), value);
		y = fmodf(static_cast<float>(y), value);
	}
}

template<typename T>
Vector2<T> Vector2<T>::operator+(void)const
{
	return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator-(void)const
{
	if (std::is_arithmetic<T>::value == true)
	{
		return Vector2(-x, -y);
	}
}

template<typename T>
T& Vector2<T>::operator[](size_t Elem)const
{
	switch (Elem)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		return x;
	}
}


// î‰är
template<typename T>
bool Vector2<T>::operator==(const Vector2<T>& value)const
{
	return (x == value.x && y == value.y);
}

template<typename T>
bool Vector2<T>::operator<(const Vector2<T>& value)const
{
	return (x < value.x && y < value.y);
}

template<typename T>
bool Vector2<T>::operator<=(const Vector2<T>& value)const
{
	return (x <= value.x && y <= value.y);
}

template<typename T>
bool Vector2<T>::operator>(const Vector2<T>& value)const
{
	return (x > value.x&& y > value.y);
}

template<typename T>
bool Vector2<T>::operator>=(const Vector2<T>& value)const
{
	return (x >= value.x && y >= value.y);
}

template<typename T>
bool Vector2<T>::operator!=(const Vector2<T>& value)const
{
	return (x != value.x || y != value.y);
}

template<typename T>
Vector2<float> Vector2<T>::NormalizedVec(void)const
{
	float d = Distance();
	if (d == 0.0f) { return { 0.0f, 0.0f }; }
	return { x / d, y / d };
}

template<typename T>
float Vector2<T>::Distance(void)const
{
	return sqrt(x * x + y * y);
}

template<typename T>
inline float Vector2<T>::SquareDistance(void) const
{
	return x * x + y * y;
}

template<typename T>
float Distance(const Vector2<T>& val1, const Vector2<T>& val2)
{
	Vector2<float> vec = Vector2<float>{ val2.x - val1.x, val2.y - val1.y };
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

template<typename T>
inline float SquareDistance(const Vector2<T>& val1, const Vector2<T>& val2)
{
	Vector2<float> vec = Vector2<float>{ val2.x - val1.x, val2.y - val1.y };
	return vec.x * vec.x + vec.y * vec.y;
}

template<typename T>
float Dot(const Vector2<T>& val1, const Vector2<T>& val2)
{
	return val1.x * val2.x + val1.y * val2.y;
}

template<typename T>
float Cross(const Vector2<T>& val1, const Vector2<T>& val2)
{
	return val1.x * val2.y - val1.y * val2.x;
}

template<typename T>
inline Vector2<T> Lerp(const Vector2<T>& val1, const Vector2<T>& val2, float t)
{
	return val1 + (val2 - val1) * t;
}

template<typename T>
Vector2<float> Vector2<T>::Reflect(const Vector2<float>& normal)
{
	Vector2<float> norm = normal.NormalizedVec();
	Vector2<float> thisVec{ static_cast<float>(this->x), static_cast<float>(this->y) };
	Vector2<float> ref = (norm * 2.0f) * Dot(norm, thisVec * -1.0f) + thisVec;
	*this = Vector2<T>{ static_cast<T>(ref.x), static_cast<T>(ref.y) };

	return ref;
}

template<typename T>
inline Vector2<T> Vector2<T>::ZeroVector(void)
{
	return Vector2<T>(0.0f, 0.0f);
}

template<typename T>
inline Vector2<float> Vector2<T>::GetNormalVec(bool positive)const
{
	if (positive)
	{
		return Vector2<float>(-y, x);
	}
	else
	{
		return Vector2<float>(y, -x);
	}
}

template<typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& val)const
{
	return { x + val.x, y + val.y, z + val.z };
}

template<typename T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& val)const
{
	return { x - val.x, y - val.y, z - val.z };
}

template<typename T>
Vector3<T> Vector3<T>::operator*(float val)const
{
	return { x * val, y * val, z * val };
}
