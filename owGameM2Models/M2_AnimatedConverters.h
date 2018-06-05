#pragma once

#include "M2_CommonTypes.h"

template <class T>
class NoConvert
{
public:
	static const T& conv(const T& t)
	{
		return t;
	}
};

class Quat16ToQuat32
{
public:
	static const Quaternion conv(const M2CompQuat t)
	{
		return Quaternion(
			float(t.x > 0 ? t.x - 32767i16 : t.x + 32767i16) / 32767.0f,
			float(t.y > 0 ? t.y - 32767i16 : t.y + 32767i16) / 32767.0f,
			float(t.z > 0 ? t.z - 32767i16 : t.z + 32767i16) / 32767.0f,
			float(t.w > 0 ? t.w - 32767i16 : t.w + 32767i16) / 32767.0f);
	}
};

class ShortToFloat
{
public:
	static const float conv(const short t)
	{
		return t / 32767.0f;
	}
};