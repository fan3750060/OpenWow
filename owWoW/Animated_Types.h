#pragma once

enum Interpolations
{
	INTERPOLATION_NONE,
	INTERPOLATION_LINEAR,
	INTERPOLATION_HERMITE
};

#include __PACK_BEGIN

struct AnimRange
{
	AnimRange(uint32 begin, uint32 end) :
		begin(begin),
		end(end)
	{}

	uint32 begin;
	uint32 end;
};

template<typename T>
struct M2Array
{
	uint32 size;
	uint32 offset; // pointer to T, relative to begin of m2 data block (i.e. MD21 chunk content or begin of file)
};

struct M2TrackBase
{
	int16 interpolation_type;
	int16 global_sequence;
	M2Array<M2Array<uint32>> timestamps;
	uint32 nRanges;
	uint32 ofsRanges;
	uint32 nTimes;
	uint32 ofsTimes;
};

template<typename T>
struct M2Track
{
	int16 interpolation_type;
	int16 global_sequence;
	uint32 nRanges;
	uint32 ofsRanges;
	uint32 nTimes;
	uint32 ofsTimes;
	uint32 nKeys;
	uint32 ofsKeys;
};

template<typename T>
struct M2TrackFake
{
	M2Array<M2Array<uint32>> timestamps;
	M2Array<M2Array<T>> values;
	uint32 nRanges;
	uint32 ofsRanges;
	uint32 nTimes;
	uint32 ofsTimes;
	uint32 nKeys;
	uint32 ofsKeys;
};

struct M2CompQuat
{
	int16 x, y, z, w;
};

#include __PACK_END

//---------------------------------------------------------//
//--                   Converters                        --//
//---------------------------------------------------------//

template <class T>
class Identity
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