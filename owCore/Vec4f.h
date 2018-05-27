#pragma once

#include "MathCommon.h"
#include "Vec3f.h"

class Vec4f
{
public:

	float x, y, z, w;

	//

	Vec4f() : x(0), y(0), z(0), w(0)
	{}

	explicit Vec4f(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w)
	{}

	explicit Vec4f(Vec3f v) : x(v.x), y(v.y), z(v.z), w(1.0f)
	{}

	explicit Vec4f(Vec3f v, float w) : x(v.x), y(v.y), z(v.z), w(w)
	{}

	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec4f operator-() const
	{
		return Vec4f(-x, -y, -z, -w);
	}

	// Vector

	Vec4f operator+(const Vec4f &v) const
	{
		return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vec4f &operator+=(const Vec4f &v)
	{
		return *this = *this + v;
	}

	Vec4f operator-(const Vec4f &v) const
	{
		return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vec4f &operator-=(const Vec4f &v)
	{
		return *this = *this - v;
	}

	// Float

	Vec4f operator*(const float f) const
	{
		return Vec4f(x * f, y * f, z * f, w * f);
	}

	Vec4f &operator*=(const float f)
	{
		return *this = *this * f;
	}

	Vec4f operator/(const float f) const
	{
		return Vec4f(x / f, y / f, z / f, w / f);
	}

	Vec4f &operator/=(const float f)
	{
		return *this = *this / f;
	}

	Vec4f& toRad()
	{
		x = degToRad(x);
		y = degToRad(y);
		z = degToRad(z);
		w = degToRad(w);
		return *this;
	}

	Vec4f toRad() const
	{
		return Vec4f(degToRad(x), degToRad(y), degToRad(z), degToRad(w));
	}

	//

	operator Vec3f()
	{
		return Vec3f(x, y, z);
	}

	operator float*()
	{
		return (float*)this;
	}
};
