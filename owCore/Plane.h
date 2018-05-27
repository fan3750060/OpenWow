#pragma once

class Plane
{
public:
	Vec3f normal;
	float dist;

	// ------------
	// Constructors
	// ------------
	Plane()
	{
		normal.x = 0; normal.y = 0; normal.z = 0; dist = 0;
	};

	explicit Plane(const float a, const float b, const float c, const float d)
	{
		normal = Vec3f(a, b, c);
		float invLen = 1.0f / normal.length();
		normal *= invLen;	// Normalize
		dist = d * invLen;
	}

	Plane(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2)
	{
		normal = v1 - v0;
		normal = normal.cross(v2 - v0);
		normal.normalize();
		dist = -normal.dot(v0);
	}

	// ----------------
	// Other operations
	// ----------------
	float distToPoint(const Vec3f &v) const
	{
		return normal.dot(v) + dist;
	}
};