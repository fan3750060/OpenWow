#pragma once

#include "MathCommon.h"

class Quaternion
{
public:
	float x, y, z, w;

	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{}

	explicit Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w)
	{}

	Quaternion(const float eulerX, const float eulerY, const float eulerZ)
	{
		Quaternion roll(sinf(eulerX * 0.5f), 0, 0, cosf(eulerX * 0.5f));
		Quaternion pitch(0, sinf(eulerY * 0.5f), 0, cosf(eulerY * 0.5f));
		Quaternion yaw(0, 0, sinf(eulerZ * 0.5f), cosf(eulerZ * 0.5f));

		// Order: y * x * z
		*this = pitch * roll * yaw;
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------

	// Quaternion

	Quaternion operator+(const Quaternion &v) const
	{
		return Quaternion(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Quaternion &operator+=(const Quaternion &v)
	{
		return *this = *this + v;
	}

	Quaternion operator-(const Quaternion &v) const
	{
		return Quaternion(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Quaternion &operator-=(const Quaternion &v)
	{
		return *this = *this - v;
	}

	Quaternion operator*(const Quaternion &q) const
	{
		return Quaternion(
			y * q.z - z * q.y + q.x * w + x * q.w,
			z * q.x - x * q.z + q.y * w + y * q.w,
			x * q.y - y * q.x + q.z * w + z * q.w,
			w * q.w - (x * q.x + y * q.y + z * q.z));
	}

	Quaternion& operator*=(const Quaternion &q)
	{
		return *this = *this * q;
	}

	// Float

	const Quaternion operator* (float d) const
	{
		Quaternion r(x*d, y*d, z*d, w*d);
		return r;
	}

	friend Quaternion operator* (float d, const Quaternion& v)
	{
		return v * d;
	}

	// ----------------
	// Other operations
	// ----------------

	Quaternion slerp(const Quaternion &q, const float t) const
	{
		// Spherical linear interpolation between two quaternions
		// Note: SLERP is not commutative

		Quaternion q1(q);

		// Calculate cosine
		float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;

		// Use the shortest path
		if (cosTheta < 0)
		{
			cosTheta = -cosTheta;
			q1.x = -q.x; q1.y = -q.y;
			q1.z = -q.z; q1.w = -q.w;
		}

		// Initialize with linear interpolation
		float scale0 = 1 - t, scale1 = t;

		// Use spherical interpolation only if the quaternions are not very close
		if ((1 - cosTheta) > 0.001f)
		{
			// SLERP
			float theta = acosf(cosTheta);
			float sinTheta = sinf(theta);
			scale0 = sinf((1 - t) * theta) / sinTheta;
			scale1 = sinf(t * theta) / sinTheta;
		}

		// Calculate final quaternion
		return Quaternion(
			x * scale0 + q1.x * scale1, 
			y * scale0 + q1.y * scale1,
			z * scale0 + q1.z * scale1, 
			w * scale0 + q1.w * scale1
		);
	}

	Quaternion nlerp(const Quaternion &q, const float t) const
	{
		// Normalized linear quaternion interpolation
		// Note: NLERP is faster than SLERP and commutative but does not yield constant velocity

		Quaternion qt;
		float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;

		// Use the shortest path and interpolate linearly
		if (cosTheta < 0)
		{
			qt = Quaternion(
				x + (-q.x - x) * t,
				y + (-q.y - y) * t,
				z + (-q.z - z) * t,
				w + (-q.w - w) * t
			);
		}
		else
		{
			qt = Quaternion(
				x + (q.x - x) * t,
				y + (q.y - y) * t,
				z + (q.z - z) * t,
				w + (q.w - w) * t
			);
		}

		// Return normalized quaternion
		float invLen = 1.0f / sqrtf(qt.x * qt.x + qt.y * qt.y + qt.z * qt.z + qt.w * qt.w);
		return Quaternion(qt.x * invLen, qt.y * invLen, qt.z * invLen, qt.w * invLen);
	}

	Quaternion inverted() const
	{
		float len = x * x + y * y + z * z + w * w;
		if (len > 0)
		{
			float invLen = 1.0f / len;
			return Quaternion(-x * invLen, -y * invLen, -z * invLen, w * invLen);
		}
		else
		{
			return Quaternion();
		}
	}

	Quaternion toXZmYW() const
	{
		return Quaternion(x, z, -y, w);
	}

	Quaternion& toXZmYW()
	{
		Quaternion v = Quaternion(x, z, -y, w);
		return *this = v;
	}
};

static inline Quaternion Fix_XZmYW(const Quaternion& _quat)
{
	return _quat.toXZmYW();
}