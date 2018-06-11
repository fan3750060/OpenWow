#pragma once

// Include
#include "Math.h"

struct BoundingBox
{
	BoundingBox();

	void set(cvec3 _min, cvec3 _max, bool _needConvert = false);
	void calculate(const vec3* _verts, uint32 _count, bool _needConvert = false);
	void calculateCenter();
	void clear();

	//

	Vec3f getCorner(uint32 index) const;
	void transform(const Matrix4f& m);
	bool makeUnion(const BoundingBox& b);

	Vec3f Min;
	Vec3f Max;
	Vec3f Center;
	float Radius;
};


