#pragma once

class Plane
{
public:
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

	Plane(cvec3 v0, cvec3 v1, cvec3 v2)
	{
		normal = v1 - v0;
		normal = normal.cross(v2 - v0);
		normal.normalize();
		dist = -normal.dot(v0);
	}

	// ----------------
	// Other operations
	// ----------------
	float distToPoint(cvec3 v) const
	{
		return normal.dot(v) + dist;
	}

public:
	Vec3f normal;
	float dist;
};