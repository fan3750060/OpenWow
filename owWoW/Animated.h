#pragma once

#include "Animated_Types.h"

// interpolation functions
template<class T>
inline T interpolate(const float r, const T &v1, const T &v2)
{
	return v1 * (1.0f - r) + v2 * r;
}

template<class T>
inline T interpolateHermite(const float r, const T &v1, const T &v2, const T &in, const T &out)
{
	// dummy
	//return interpolate<T>(r,v1,v2);

	// basis functions
	float h1 = 2.0f*r*r*r - 3.0f*r*r + 1.0f;
	float h2 = -2.0f*r*r*r + 3.0f*r*r;
	float h3 = r * r*r - 2.0f*r*r + r;
	float h4 = r * r*r - r * r;

	// interpolation
	return v1 * h1 + v2 * h2 + in * h3 + out * h4;
}

// "linear" interpolation for quaternions should be slerp by default
template<>
inline Quaternion interpolate<Quaternion>(const float r, const Quaternion &v1, const Quaternion &v2)
{
	return v1.slerp(v2, r);
}


/*
	Generic animated value class:

	T is the data type to animate
	D is the data type stored in the file (by default this is the same as T)
	Conv is a conversion object that defines T conv(D) to convert from D to T (by default this is an identity function)	(there might be a nicer way to do this? meh meh)
*/
template <class T, class D = T, class Conv = Identity<T> >
class Animated
{
public:
	bool uses(uint32 anim)
	{
		return interpolation_type != INTERPOLATION_NONE;
	}

	T getValue(uint32 anim, uint32 time, uint32 globalTime)
	{
		if (interpolation_type == INTERPOLATION_NONE)
		{
			return data[0];
		}

		AnimRange range(0, data.size() - 1);

		// obtain a time value and a data range
		if (global_sequence != -1)
		{
			if (globals[global_sequence] == 0)
			{
				time = 0;
			}
			else
			{
				time = globalTime % globals[global_sequence];
			}
		}
		else
		{
			range = ranges[anim];
			time %= times[times.size() - 1];
		}

		if (range.begin != range.end)
		{
			// Get pos by time
			uint32 pos = 0;
			for (uint32 i = range.begin; i < range.end; i++)
			{
				if (time >= times[i] && time < times[i + 1])
				{
					pos = i;
					break;
				}
			}

			uint32 t1 = times[pos];
			uint32 t2 = times[pos + 1];
			float r = (time - t1) / (float)(t2 - t1);

			if (interpolation_type == INTERPOLATION_LINEAR)
			{
				return interpolate<T>(r, data[pos], data[pos + 1]);
			}
			else
			{
				return interpolateHermite<T>(r, data[pos], data[pos + 1], in[pos], out[pos]);
			}
		}
		else
		{
			return data[range.begin];
		}
	}

	void init(M2Track<D>& b, IFile* f, uint32* gs)
	{
		globals = gs;
		interpolation_type = b.interpolation_type;
		global_sequence = b.global_sequence;
		if (global_sequence != -1)
		{
			assert1(gs);
		}

		// Fix ranges
		if (b.nRanges <= 0 && interpolation_type != 0 && global_sequence == -1)
		{
			ranges.push_back(AnimRange(0, b.nKeys - 1));
		}

		// ranges
		uint32* pranges = (uint32*)(f->GetData() + b.ofsRanges);
		for (uint32 i = 0; i < b.nRanges; i++) ranges.push_back(AnimRange(pranges[i], pranges[i + 1]));

		// times
		assert1(b.nTimes == b.nKeys);
		uint32 *ptimes = (uint32*)(f->GetData() + b.ofsTimes);
		for (uint32 i = 0; i < b.nTimes; i++) times.push_back(ptimes[i]);

		// keyframes
		D *keys = (D*)(f->GetData() + b.ofsKeys);
		switch (interpolation_type)
		{
		case INTERPOLATION_NONE:
		case INTERPOLATION_LINEAR:
			for (uint32 i = 0; i < b.nKeys; i++)
			{
				data.push_back(Conv::conv(keys[i]));
			}
			break;

		case INTERPOLATION_HERMITE:
			for (uint32 i = 0; i < b.nKeys; i++)
			{
				data.push_back(Conv::conv(keys[i * 3]));
				in.push_back(Conv::conv(keys[i * 3 + 1]));
				out.push_back(Conv::conv(keys[i * 3 + 2]));
			}
			break;
		}
	}

	void fix(T fixfunc(const T&))
	{
		switch (interpolation_type) {
		case INTERPOLATION_NONE:
		case INTERPOLATION_LINEAR:
			for (uint32 i = 0; i < data.size(); i++)
			{
				data[i] = fixfunc(data[i]);
			}
			break;

		case INTERPOLATION_HERMITE:
			for (uint32 i = 0; i < data.size(); i++)
			{
				data[i] = fixfunc(data[i]);
				in[i] = fixfunc(in[i]);
				out[i] = fixfunc(out[i]);
			}
			break;
		}
	}

private:
	uint32 interpolation_type;
	uint32 global_sequence;
	uint32* globals;

	std::vector<AnimRange> ranges;
	std::vector<int32> times;
	std::vector<T> data;

	// Hermite interpolation
	std::vector<T> in;
	std::vector<T> out;
};
