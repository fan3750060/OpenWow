#pragma once

#include "M2_AnimatedConverters.h"
#include "M2_Types.h"

// interpolation functions
template<class T>
inline T interpolate(const float r, const T& v1, const T& v2)
{
	return v1 * (1.0f - r) + v2 * r;
}

// "linear" interpolation for quaternions should be slerp by default
template<>
inline Quaternion interpolate<Quaternion>(const float r, cquat v1, cquat v2)
{
	return v1.slerp(v2, r);
}

template<class T>
inline T interpolateHermite(const float r, const T& v1, const T& v2, const T& in, const T& out)
{
	// dummy
	return interpolate<T>(r, v1, v2);

	float rPow3 = r * r * r;
	float rPow2 = r * r;

	// basis functions
	float h1 = 2.0f*rPow3 - 3.0f*rPow2 + 1.0f;
	float h2 = -2.0f*rPow3 + 3.0f*rPow2;
	float h3 = rPow3 - 2.0f*rPow2 + r;
	float h4 = rPow3 - rPow2;

	// interpolation
	return v1 * h1 + v2 * h2 + in * h3 + out * h4;
}

template<class T>
inline T interpolateBezier(const float r, const T& v1, const T& v2, const T& in, const T& out)
{
	float InverseFactor = (1.0f - r);
	float FactorTimesTwo = r * r;
	float InverseFactorTimesTwo = InverseFactor * InverseFactor;

	// basis functions
	float h1 = InverseFactorTimesTwo * InverseFactor;
	float h2 = 3.0f * r * InverseFactorTimesTwo;
	float h3 = 3.0f * FactorTimesTwo * InverseFactor;
	float h4 = FactorTimesTwo * r;

	// interpolation
	return static_cast<T>(v1*h1 + v2 * h2 + in * h3 + out * h4);
}

template<class T>
static inline T NoFix(const T& _v)
{
	return _v;
}


/*
	Generic animated value class:

	T is the values type to animate
	D is the values type stored in the file (by default this is the same as T)
	Conv is a conversion object that defines T conv(D) to convert from D to T (by default this is an identity function)	(there might be a nicer way to do this? meh meh)
*/
template <class T, class D = T, class Conv = NoConvert<T> >
class M2_Animated
{
public:
	void init(const M2Track<D>& b, IFile* f, cGlobalLoopSeq _globalSec, T fixfunc(const T&) = NoFix)
	{
		m_Type = b.interpolation_type;
		m_GlobalSecIndex = b.global_sequence;
		m_GlobalSec = _globalSec;

		// If hasn't index, then use global sec
		if (m_GlobalSecIndex != -1)
		{
			assert1(m_GlobalSec != nullptr);
		}

		assert1((b.interpolation_ranges.size > 0) || (m_GlobalSecIndex != -1) || (m_Type == INTERPOLATION_NONE));

		// ranges
		if (b.interpolation_ranges.size > 0)
		{
			uint32* ranges = (uint32*)(f->GetData() + b.interpolation_ranges.offset);
			for (uint32 i = 0; i < b.interpolation_ranges.size; i += 2)
			{
				m_Ranges.push_back(make_pair(ranges[i], ranges[i + 1]));
			}
		}

		// times
		uint32* times = (uint32*)(f->GetData() + b.timestamps.offset);
		for (uint32 i = 0; i < b.timestamps.size; i++)
		{
			m_Times.push_back(times[i]);
		}

		assert1(b.timestamps.size == b.values.size);

		// keyframes
		D* values = (D*)(f->GetData() + b.values.offset);
		for (uint32 i = 0; i < b.values.size; i++)
		{
			switch (m_Type)
			{
			case INTERPOLATION_LINEAR:
				m_Values.push_back(fixfunc(Conv::conv(values[i])));
				break;

			case INTERPOLATION_HERMITE:
				m_Values.push_back(fixfunc(Conv::conv(values[i * 3 + 0])));
				m_ValuesHermiteIn.push_back(fixfunc(Conv::conv(values[i * 3 + 1])));
				m_ValuesHermiteOut.push_back(fixfunc(Conv::conv(values[i * 3 + 2])));
				break;
			}
		}

	}

	bool uses(uint16 anim) const
	{
		if (m_Type == INTERPOLATION_NONE)
		{
			return false;
		}

		if (m_GlobalSecIndex == -1 && m_Ranges.size() <= anim)
		{
			return false;
		}

		return true;
	}

	T getValue(uint16 anim, uint32 time, uint32 globalTime) const
	{
		assert1(m_Type != INTERPOLATION_NONE);

		pair<uint32, uint32> range = make_pair(0, m_Values.size() - 1);

		// obtain a time value and a values range
		if (m_GlobalSecIndex != -1)
		{
			if (m_GlobalSec->at(m_GlobalSecIndex).timestamp == 0)
			{
				time = 0;
			}
			else
			{
				time = globalTime % m_GlobalSec->at(m_GlobalSecIndex).timestamp;
			}
		}
		else
		{
			assert1(time >= m_Times[0] && time < m_Times[m_Times.size() - 1]);
			range = m_Ranges[anim];
		}



		// If simple frame
		if (range.first == range.second)
		{
			return m_Values[range.first];
		}

		// Get pos by time
		uint32 pos = UINT32_MAX;
		for (uint32 i = range.first; i < range.second; i++)
		{
			if (time >= m_Times[i] && time < m_Times[i + 1])
			{
				pos = i;
				break;
			}
		}
		assert1(pos != UINT32_MAX);

		uint32 t1 = m_Times[pos];
		uint32 t2 = m_Times[pos + 1];
		assert1(t2 > t1);
		assert1(time >= t1 && time < t2);
		float r = (float)(time - t1) / (float)(t2 - t1);

		switch (m_Type)
		{
		case INTERPOLATION_LINEAR:
			return interpolate<T>(r, m_Values[pos], m_Values[pos + 1]);
			break;

		case INTERPOLATION_HERMITE:
			return interpolateHermite<T>(r, m_Values[pos], m_Values[pos + 1], m_ValuesHermiteIn[pos], m_ValuesHermiteOut[pos]);
			break;
		}

		return m_Values[0];
	}

private:
	Interpolations				m_Type;
	int16						m_GlobalSecIndex;
	cGlobalLoopSeq				m_GlobalSec;

	vector<pair<uint32, uint32>>m_Ranges;
	vector<int32>				m_Times;

	vector<T>					m_Values;
	vector<T>					m_ValuesHermiteIn;
	vector<T>					m_ValuesHermiteOut;
};
