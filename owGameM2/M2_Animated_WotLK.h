#pragma once

#include "M2_AnimatedConverters.h"
#include "M2_Types.h"

//

#define	MAX_ANIMATED (500u)

/*
	Generic animated value class:

	T is the data type to animate
	D is the data type stored in the file (by default this is the same as T)
	Conv is a conversion object that defines T conv(D) to convert from D to T (by default this is an identity function)	(there might be a nicer way to do this? meh meh)
*/
template <class T, class D = T, class Conv = NoConvert<T> >
class M2_Animated
{
public:
	void init(const M2Track<D>& b, IFile* f, cGlobalLoopSeq _globalSec, T fixfunc(const T&) = NoFix)
	{
		m_GlobalSec = _globalSec;
		interpolation_type = b.interpolation_type;
		global_sequence = b.global_sequence;
		if (global_sequence != -1)
		{
			assert1(_globalSec);
		}

		assert1(b.timestamps.size == b.values.size);
		sizes = b.timestamps.size;
		if (b.timestamps.size == 0)	return;

		// times
		for (size_t j = 0; j < b.timestamps.size; j++)
		{
			M2Array<uint32>* pHeadTimes = (M2Array<uint32>*)(f->getData() + b.timestamps.offset + j * sizeof(M2Array<uint32>));

			uint32* ptimes = (uint32*)(f->getData() + pHeadTimes->offset);
			for (size_t i = 0; i < pHeadTimes->size; i++)
			{
				times[j].push_back(ptimes[i]);
			}
		}

		// keyframes
		for (size_t j = 0; j < b.values.size; j++)
		{
			M2Array<D>* pHeadKeys = (M2Array<D>*)(f->getData() + b.values.offset + j * sizeof(M2Array<D>));

			D* values = (D*)(f->getData() + pHeadKeys->offset);
			for (uint32 i = 0; i < pHeadKeys->size; i++)
			{
				switch (interpolation_type)
				{
				case INTERPOLATION_LINEAR:
					data[j].push_back(fixfunc(Conv::conv(values[i])));
					break;

				case INTERPOLATION_HERMITE:
					data[j].push_back(fixfunc(Conv::conv(values[i * 3 + 0])));
					in[j].push_back(fixfunc(Conv::conv(values[i * 3 + 1])));
					out[j].push_back(fixfunc(Conv::conv(values[i * 3 + 2])));
					break;
				}
			}
		}
	}

	void init(const M2Track<D>& b, IFile* f, cGlobalLoopSeq _globalSec, IFile** animfiles, T fixfunc(const T&) = NoFix)
	{
		m_GlobalSec = _globalSec;
		interpolation_type = b.interpolation_type;
		global_sequence = b.global_sequence;
		if (global_sequence != -1)
		{
			assert1(_globalSec);
		}

		assert1(b.timestamps.size == b.values.size);
		sizes = b.timestamps.size;
		if (b.timestamps.size == 0)	return;

		// times
		for (size_t j = 0; j < b.timestamps.size; j++)
		{
			M2Array<uint32>* pHeadTimes = (M2Array<uint32>*)(f->getData() + b.timestamps.offset + j * sizeof(M2Array<uint32>));

			uint32* ptimes;
			if (animfiles[j] != nullptr)
				ptimes = (uint32*)(animfiles[j]->getData() + pHeadTimes->offset);
			else
				ptimes = (uint32*)(f->getData() + pHeadTimes->offset);

			for (size_t i = 0; i < pHeadTimes->size; i++)
			{
				times[j].push_back(ptimes[i]);
			}
		}

		// keyframes
		for (size_t j = 0; j < b.values.size; j++)
		{
			M2Array<D>* pHeadKeys = (M2Array<D>*)(f->getData() + b.values.offset + j * sizeof(M2Array<D>));
			assert1((D*)(f->getData() + pHeadKeys->offset));

			D *keys;
			if (animfiles[j] != nullptr)
				keys = (D*)(animfiles[j]->getData() + pHeadKeys->offset);
			else
				keys = (D*)(f->getData() + pHeadKeys->offset);

			switch (interpolation_type)
			{
			case INTERPOLATION_NONE:
			case INTERPOLATION_LINEAR:
				for (size_t i = 0; i < pHeadKeys->size; i++)
					data[j].push_back(fixfunc(Conv::conv(keys[i])));
				break;

			case INTERPOLATION_HERMITE:
				for (size_t i = 0; i < pHeadKeys->size; i++)
				{
					data[j].push_back(fixfunc(Conv::conv(keys[i * 3 + 0])));
					in[j].push_back(fixfunc(Conv::conv(keys[i * 3 + 1])));
					out[j].push_back(fixfunc(Conv::conv(keys[i * 3 + 2])));
				}
				break;
			}
		}
	}

	bool uses(uint32 anim) const
	{
		if (global_sequence > -1)
		{
			anim = 0;
		}

		return (data[anim].size() > 0);
	}

	T getValue(uint32 anim, uint32 time, uint32 globalTime)
	{
		if (global_sequence > -1)
		{
			if (m_GlobalSec->at(global_sequence).timestamp == 0)
			{
				time = 0;
			}
			else
			{
				time = globalTime % m_GlobalSec->at(global_sequence).timestamp;
			}
			anim = 0;
		}

		if (data[anim].size() > 1 && times[anim].size() > 1)
		{
			size_t t1, t2;
			size_t pos = 0;
			int max_time = times[anim][times[anim].size() - 1];
			if (max_time > 0)
			{
				time %= max_time; // I think this might not be necessary?
			}
			for (size_t i = 0; i < times[anim].size() - 1; i++)
			{
				if (time >= times[anim][i] && time < times[anim][i + 1])
				{
					pos = i;
					break;
				}
			}
			t1 = times[anim][pos];
			t2 = times[anim][pos + 1];
			float r = (time - t1) / (float)(t2 - t1);

			if (interpolation_type == INTERPOLATION_LINEAR)
			{
				return interpolate<T>(r, data[anim][pos], data[anim][pos + 1]);
			}
			else if (interpolation_type == INTERPOLATION_NONE)
			{
				return data[anim][pos];
			}
			else
			{
				// INTERPOLATION_HERMITE is only used in cameras afaik?
				return interpolateHermite<T>(r, data[anim][pos], data[anim][pos + 1], in[anim][pos], out[anim][pos]);
			}
		}
		else
		{
			// default value
			if (data[anim].size() == 0)
			{
				return T();
			}
			else
			{
				return data[anim][0];
			}
		}
	}

private:
	int32 interpolation_type;
	int32 global_sequence;
	cGlobalLoopSeq				m_GlobalSec;

	size_t sizes;

	vector<int32> times[MAX_ANIMATED];
	vector<T> data[MAX_ANIMATED];
	vector<T> in[MAX_ANIMATED];
	vector<T> out[MAX_ANIMATED];
};