#pragma once

#include "MathCommon.h"

class Vec2f
{
public:
	float x, y;


	// ------------
	// Constructors
	// ------------
	Vec2f() : x(0.0f), y(0.0f)
	{}

	explicit Vec2f(Math::NoInitHint)
	{
		// Constructor without default initialization
	}

	Vec2f(const float x, const float y) : x(x), y(y)
	{}

	Vec2f(const Vec2f &v) : x(v.x), y(v.y)
	{}

	// ------
	// Access
	// ------
	float operator[](unsigned int index) const
	{
		return *(&x + index);
	}

	float &operator[](unsigned int index)
	{
		return *(&x + index);
	}

	// -----------
	// Comparisons
	// -----------
	bool operator==(const Vec2f &v) const
	{
		return (x > v.x - Math::Epsilon && x < v.x + Math::Epsilon &&
			y > v.y - Math::Epsilon && y < v.y + Math::Epsilon);
	}

	bool operator!=(const Vec2f &v) const
	{
		return (x < v.x - Math::Epsilon || x > v.x + Math::Epsilon ||
			y < v.y - Math::Epsilon || y > v.y + Math::Epsilon);
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------

	Vec2f operator-() const
	{
		return Vec2f(-x, -y);
	}

	// Vector

	Vec2f operator+(const Vec2f &v) const
	{
		return Vec2f(x + v.x, y + v.y);
	}

	Vec2f &operator+=(const Vec2f &v)
	{
		return *this = *this + v;
	}

	Vec2f operator-(const Vec2f &v) const
	{
		return Vec2f(x - v.x, y - v.y);
	}

	Vec2f &operator-=(const Vec2f &v)
	{
		return *this = *this - v;
	}

	Vec2f operator*(const Vec2f &v) const
	{
		Vec2f(x * v.x, y * v.y);
	}

	Vec2f &operator*=(const Vec2f &v)
	{
		return *this = *this * v;
	}

	Vec2f operator/(const Vec2f &v) const
	{
		return Vec2f(x / v.x, y / v.y);
	}

	Vec2f& operator/=(const Vec2f &v)
	{
		return *this = *this / v;
	}

	// Float

	Vec2f operator+(const float f) const
	{
		return Vec2f(x + f, y + f);
	}

	Vec2f &operator+=(const float f)
	{
		return *this = *this + f;
	}

	Vec2f operator-(const float f) const
	{
		return Vec2f(x - f, y - f);
	}

	Vec2f &operator-=(const float f)
	{
		return *this = *this - f;
	}

	Vec2f operator*(const float f) const
	{
		return Vec2f(x * f, y * f);
	}

	Vec2f &operator*=(const float f)
	{
		return *this = *this * f;
	}

	Vec2f operator/(const float f) const
	{
		return Vec2f(x / f, y / f);
	}

	Vec2f& operator/=(const float f)
	{
		return *this = *this / f;
	}

	// ----------------
	// Special products
	// ----------------
	float dot(const Vec2f &v) const
	{
		return x * v.x + y * v.y;
	}

	// ----------------
	// Other operations
	// ----------------
	float length() const
	{
		return sqrtf(x * x + y * y);
	}

	float length2() const
	{
		return x * x + y * y;
	}

	Vec2f normalized() const
	{
		float invLen = 1.0f / length();
		return Vec2f(x * invLen, y * invLen);
	}

	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
	}

	Vec2f lerp(const Vec2f &v, float f) const
	{
		return Vec2f(x + (v.x - x) * f, y + (v.y - y) * f);
	}

	Vec2f& toRad()
	{
		x = degToRad(x);
		y = degToRad(y);
		return *this;
	}

	Vec2f toRad() const
	{
		return Vec2f(degToRad(x), degToRad(y));
	}

	//

	operator float*()
	{
		return (float*)this;
	}
};