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

	Plane(cvec3 c, cvec3 b, cvec3 a)
	{
		vec3 cb = b - c;
		vec3 ca = a - c;
		normal = cb.cross(ca);
		normal.normalize();
		dist = -normal.dot(c);
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

inline bool cullPolyByPlanes(const Plane* _planes, uint32 _planesCount, const vec3* verts, uint32 vertsCount)
{
	for (uint32 i = 0; i < _planesCount; ++i)
	{
		bool allOut = true;

		for (uint32 j = 0; j < vertsCount; ++j)
		{
			if (_planes[i].distToPoint(verts[j]) < 0)
			{
				allOut = false;
				break;
			}
		}

		if (allOut) return true;
	}

	return false;
}