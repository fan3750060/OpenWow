#pragma once

#include "M2_AnimatedConverters.h"
#include "M2_Types.h"

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
	M2_Animated() :
		m_Times(nullptr),
		m_Data(nullptr),
		m_DataIN(nullptr),
		m_DataOUT(nullptr)
	{}
	~M2_Animated()
	{
		if (m_Times != nullptr) delete[] m_Times;
		if (m_Data != nullptr) delete[] m_Data;
		if (m_DataIN != nullptr) delete[] m_DataIN;
		if (m_DataOUT != nullptr) delete[] m_DataOUT;
	}

	void init(const M2Track<D>& b, std::shared_ptr<IFile> f, cGlobalLoopSeq _globalSec, T fixfunc(const T&) = NoFix, std::vector<std::shared_ptr<IFile>>* animfiles = nullptr)
	{
		m_Type = b.interpolation_type;
		m_GlobalSeqIndex = b.global_sequence;
		m_GlobalSec = _globalSec;
		if (m_GlobalSeqIndex != -1)
		{
			_ASSERT(_globalSec);
		}

		// Checks
		_ASSERT(b.timestamps.size == b.values.size);
		m_Count = b.timestamps.size;
		if (b.timestamps.size == 0)	return;

		m_Times = new std::vector<uint32>[m_Count];
		m_Data = new std::vector<T>[m_Count];
		if (m_Type == INTERPOLATION_HERMITE)
		{
			m_DataIN = new std::vector<T>[m_Count];
			m_DataOUT = new std::vector<T>[m_Count];
		}

		// times
		M2Array<uint32>* pHeadTimes = (M2Array<uint32>*)(f->getData() + b.timestamps.offset);
		M2Array<D>* pHeadKeys = (M2Array<D>*)(f->getData() + b.values.offset);
		for (uint32 j = 0; j < m_Count; j++)
		{
			uint32* times = nullptr;
			D* keys = nullptr;
			if (animfiles != nullptr && animfiles->at(j) != nullptr)
			{
				assert3(pHeadTimes[j].offset < animfiles->at(j)->getSize(), std::to_string(pHeadTimes[j].offset).c_str(), std::to_string(animfiles->at(j)->getSize()).c_str());
				times = (uint32*)(animfiles->at(j)->getData() + pHeadTimes[j].offset);

				assert3(pHeadKeys[j].offset < animfiles->at(j)->getSize(), std::to_string(pHeadKeys[j].offset).c_str(), std::to_string(animfiles->at(j)->getSize()).c_str());
				keys = (D*)(animfiles->at(j)->getData() + pHeadKeys[j].offset);
			}
			else
			{
				assert3(pHeadTimes[j].offset < f->getSize(), std::to_string(pHeadTimes[j].offset).c_str(), std::to_string(f->getSize()).c_str());
				times = (uint32*)(f->getData() + pHeadTimes[j].offset);

				assert3(pHeadKeys[j].offset < f->getSize(), std::to_string(pHeadKeys[j].offset).c_str(), std::to_string(f->getSize()).c_str());
				keys = (D*)(f->getData() + pHeadKeys[j].offset);
			}


			_ASSERT(times != nullptr);
			for (uint32 i = 0; i < pHeadTimes[j].size; i++)
			{
				m_Times[j].push_back(times[i]);
			}

			_ASSERT(keys != nullptr);
			for (uint32 i = 0; i < pHeadKeys[j].size; i++)
			{
				switch (m_Type)
				{
				case INTERPOLATION_NONE:
				case INTERPOLATION_LINEAR:
				{
					m_Data[j].push_back(fixfunc(Conv::conv(keys[i])));
				}
				break;

				case INTERPOLATION_HERMITE:
				{
					m_Data[j].push_back(fixfunc(Conv::conv(keys[i * 3 + 0])));
					m_DataIN[j].push_back(fixfunc(Conv::conv(keys[i * 3 + 1])));
					m_DataOUT[j].push_back(fixfunc(Conv::conv(keys[i * 3 + 2])));
				}
				break;
				}
			}
		}
	}

	bool uses(uint32 anim) const
	{
		if (m_GlobalSeqIndex != -1)
		{
			anim = 0;
		}

		if (m_Count <= anim)
		{
			return false;
		}

		return (m_Data[anim].size() > 0);
	}

	T getValue(uint32 anim, uint32 time, uint32 globalTime) const
	{
		if (m_GlobalSeqIndex != -1)
		{
			if (m_GlobalSec->at(m_GlobalSeqIndex).timestamp == 0)
			{
				time = 0;
			}
			else
			{
				time = globalTime % m_GlobalSec->at(m_GlobalSeqIndex).timestamp;
			}
			anim = 0;
		}

		// TODO: Delete me
		if (anim >= m_Count)
		{
			return T();
		}

		const std::vector<uint32>& pTimes = m_Times[anim];
		const std::vector<T>& pData = m_Data[anim];

		if ((m_Count > anim) && (pData.size() > 1) && (pTimes.size() > 1))
		{
			int max_time = pTimes.at(pTimes.size() - 1);
			if (max_time > 0)
			{
				time %= max_time; // I think this might not be necessary?
			}

			uint32 pos = 0;
			for (uint32 i = 0; i < pTimes.size() - 1; i++)
			{
				if (time >= pTimes.at(i) && time < pTimes.at(i + 1))
				{
					pos = i;
					break;
				}
			}

			uint32 t1 = pTimes.at(pos);
			uint32 t2 = pTimes.at(pos + 1);
			float r = (time - t1) / (float)(t2 - t1);

			switch (m_Type)
			{
			case INTERPOLATION_NONE:
				return pData.at(pos);
				break;

			case INTERPOLATION_LINEAR:
				return interpolate<T>(r, pData.at(pos), pData.at(pos + 1));
				break;

			case INTERPOLATION_HERMITE:
				return interpolateHermite<T>(r, pData.at(pos), pData.at(pos + 1), m_DataIN[anim].at(pos), m_DataOUT[anim].at(pos));
				break;
			}
		}
		else if (!(pData.empty()))
		{
			return pData.at(0);
		}

		return T();
	}

private:
	Interpolations	m_Type;
	int32			m_GlobalSeqIndex;
	cGlobalLoopSeq	m_GlobalSec;

	uint32			m_Count;

	std::vector<uint32>*	m_Times;
	std::vector<T>*			m_Data;
	std::vector<T>*			m_DataIN;
	std::vector<T>*			m_DataOUT;
};