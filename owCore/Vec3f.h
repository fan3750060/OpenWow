#pragma once

#include "MathCommon.h"

class Vec3f
{
public:
	float x, y, z;

	//

	// ------------
	// Constructors
	// ------------
	Vec3f() : x(0.0f), y(0.0f), z(0.0f)
	{}

	explicit Vec3f(Math::NoInitHint)
	{
		// Constructor without default initialization
	}

	Vec3f(const float v) : x(v), y(v), z(v)
	{}

	Vec3f(const float x, const float y, const float z) : x(x), y(y), z(z)
	{}

	Vec3f(const Vec3f &v) : x(v.x), y(v.y), z(v.z)
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
	bool operator==(const Vec3f &v) const
	{
		return (x > v.x - Math::Epsilon && x < v.x + Math::Epsilon &&
			y > v.y - Math::Epsilon && y < v.y + Math::Epsilon &&
			z > v.z - Math::Epsilon && z < v.z + Math::Epsilon);
	}

	bool operator!=(const Vec3f &v) const
	{
		return (x < v.x - Math::Epsilon || x > v.x + Math::Epsilon ||
			y < v.y - Math::Epsilon || y > v.y + Math::Epsilon ||
			z < v.z - Math::Epsilon || z > v.z + Math::Epsilon);
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec3f operator-() const
	{
		return Vec3f(-x, -y, -z);
	}

	// Vector

	Vec3f operator+(const Vec3f &v) const
	{
		return Vec3f(x + v.x, y + v.y, z + v.z);
	}

	Vec3f &operator+=(const Vec3f &v)
	{
		return *this = *this + v;
	}

	Vec3f operator-(const Vec3f &v) const
	{
		return Vec3f(x - v.x, y - v.y, z - v.z);
	}

	Vec3f &operator-=(const Vec3f &v)
	{
		return *this = *this - v;
	}

	Vec3f operator*(const Vec3f &v) const
	{
		return Vec3f(x * v.x, y * v.y, z / v.z);
	}

	Vec3f &operator*=(const Vec3f &v)
	{
		return *this = *this * v;
	}

	Vec3f operator/(const Vec3f &v) const
	{
		return Vec3f(x / v.x, y / v.y, z / v.z);
	}

	Vec3f& operator/=(const Vec3f &v)
	{
		return *this = *this / v;
	}

	// Float

	Vec3f operator*(const float f) const
	{
		return Vec3f(x * f, y * f, z * f);
	}

	Vec3f &operator*=(const float f)
	{
		return *this = *this * f;
	}

	Vec3f operator/(const float f) const
	{
		return Vec3f(x / f, y / f, z / f);
	}

	Vec3f &operator/=(const float f)
	{
		return *this = *this / f;
	}

	// ----------------
	// Special products
	// ----------------
	float dot(const Vec3f &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3f cross(const Vec3f &v) const
	{
		return Vec3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	// ----------------
	// Other operations
	// ----------------
	float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float length2() const
	{
		return x * x + y * y + z * z;
	}

	Vec3f normalized() const
	{
		float invLen = 1.0f / length();
		return Vec3f(x * invLen, y * invLen, z * invLen);
	}

	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}

	Vec3f toRotation() const
	{
		// Assumes that the unrotated view vector is (0, 0, -1)
		Vec3f v;

		if (y != 0)
			v.x = atan2f(y, sqrtf(x*x + z * z));

		if (x != 0 || z != 0)
			v.y = atan2f(-x, -z);

		return v;
	}

	Vec3f lerp(const Vec3f &v, float f) const
	{
		return Vec3f(x + (v.x - x) * f, y + (v.y - y) * f, z + (v.z - z) * f);
	}

	Vec3f& toRad()
	{
		x = degToRad(x);
		y = degToRad(y);
		z = degToRad(z);
		return *this;
	}

	Vec3f toRad() const
	{
		return Vec3f(degToRad(x), degToRad(y), degToRad(z));
	}

	Vec3f toXZY() const
	{
		return Vec3f(x, z, y);
	}

	Vec3f& toXZY()
	{
		Vec3f v = Vec3f(x, z, y);
		return *this = v;
	}

	Vec3f toXZmY() const
	{
		return Vec3f(x, z, -y);
	}

	Vec3f& toXZmY()
	{
		Vec3f v = Vec3f(x, z, -y);
		return *this = v;
	}

	//

	operator float*()
	{
		return (float*)this;
	}
};


static inline Vec3f Fix_XZY(const Vec3f& _vec)
{
	return _vec.toXZY();
}

static inline Vec3f Fix_XZmY(const Vec3f& _vec)
{
	return _vec.toXZmY();
}
