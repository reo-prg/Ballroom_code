#pragma once
#include <cmath>
#include <type_traits>
#include <limits>

template<typename T>
class Vector2
{
public:
	T x, y;

	Vector2()
	{ 
		Vector2<T>::x = T();  Vector2<T>::y = T();
	};
	Vector2(T x, T y)
	{ 
		Vector2<T>::x = x;  Vector2<T>::y = y;
	};

	~Vector2();

	Vector2(const Vector2<T>& value) = default;
	Vector2<T>& operator=(const Vector2<T>& value) = default;

	Vector2(Vector2<T>&& value) = default;
	Vector2<T>& operator=(Vector2<T>&& value) = default;

	Vector2<T> operator+(const Vector2<T>& value)const;

	Vector2<T> operator+(const T& value)const;

	Vector2<T> operator-(const Vector2<T>& value)const noexcept;

	Vector2<T> operator*(const Vector2<T>& value)const noexcept;

	Vector2<T> operator*(float value)const noexcept;

	Vector2<T> operator/(const Vector2<T>& value)const;

	Vector2<T> operator/(float value)const;

	Vector2<T> operator%(float value);

	Vector2<T> operator%(int value);

	void operator+= (const Vector2<T>& value);

	void operator+=(const T& value);

	void operator-=(const Vector2<T>& value)noexcept;

	void operator*=(const Vector2<T>& value)noexcept;

	void operator*=(float value)noexcept;

	void operator/=(const Vector2<T>& value);

	void operator/=(float value);

	void operator%= (const Vector2<T>& value);

	void operator%= (float value);

	Vector2<T> operator+(void)const;

	Vector2<T> operator-(void)const;

	T& operator[](size_t Elem)const;


	// î‰är
	bool operator==(const Vector2<T>& value)const;

	bool operator<(const Vector2<T>& value)const;

	bool operator<=(const Vector2<T>& value)const;

	bool operator>(const Vector2<T>& value)const;

	bool operator>=(const Vector2<T>& value)const;

	bool operator!=(const Vector2<T>& value)const;

	Vector2<float> NormalizedVec(void)const;

	float Distance(void)const;

	float SquareDistance(void)const;

	Vector2<float> Reflect(const Vector2<float>& normal);

	static Vector2<T> ZeroVector(void);

	Vector2<float> GetNormalVec(bool positive = true)const;
};

template<typename T>
Vector2<T> operator*(float value, const Vector2<T>& value2)
{
	if (std::is_arithmetic<T>::value == true)
	{
		return Vector2Template<T>(value * value.x, value * value.y);
	}
	else
	{
		return value2;
	}
}

template<typename T>
float Distance(const Vector2<T>& val1, const Vector2<T>& val2);

template<typename T>
float SquareDistance(const Vector2<T>& val1, const Vector2<T>& val2);

template<typename T>
float Dot(const Vector2<T>& val1, const Vector2<T>& val2);

template<typename T>
float Cross(const Vector2<T>& val1, const Vector2<T>& val2);

template<typename T>
Vector2<T> Lerp(const Vector2<T>& val1, const Vector2<T>& val2, float t);

using Vector2Int = Vector2<int>;


template<typename T>
class Vector3
{
public:
	T x, y, z;

	Vector3() { Vector3<T>::x = T();  Vector3<T>::y = T(); Vector3<T>::z = T(); };
	Vector3(T x, T y, T z) { Vector3<T>::x = x; Vector3<T>::y = y; Vector3<T>::z = z; };
	~Vector3() = default;

	Vector3(const Vector3<T>&) = default;
	Vector3<T>& operator=(const Vector3<T>&) = default;

	Vector3(Vector3<T>&&) = default;
	Vector3<T>& operator=(Vector3<T>&&) = default;

	Vector3<T> operator+(const Vector3<T>& val)const;
	Vector3<T> operator-(const Vector3<T>& val)const;
	Vector3<T> operator*(float val)const;
};

#include "Module/Vector.h"
