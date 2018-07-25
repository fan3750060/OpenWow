#include "stdafx.h"

// General
#include "Perfomance.h"

Perfomance* _Perfomance;

Perfomance::Perfomance() :
	m_UIMgr(GetManager<IUIMgr>())
{
	FrameBegin();
}

void Perfomance::FrameBegin()
{
	for (uint8 i = 0; i < PERF_TIMERS_COUNT; i++)
	{
		m_Timer[i] = 0.0;
		m_TimerCurrent[i] = 0.0;
		m_DrawCount[i] = 0;
	}

	//Start(PERF_GLOBAL);
}

void Perfomance::FrameEnd()
{
	//Stop(PERF_GLOBAL);
}

string Perfomance::Sum()
{
	double timerSum = 0.0;
	uint32 drawSum = 0;

	for (uint8 i = 0; i < PERF_TIMERS_COUNT; i++)
	{
		timerSum += m_Timer[i];
		drawSum += m_DrawCount[i];
	}

	return "t: [" + to_string(static_cast<uint32>(timerSum * 1000.0)) + "] c: [" + to_string(drawSum) + "]";
}

void Perfomance::Render(cvec2 _pos)
{
	vec2 point = _pos;
	float diff = 15.0f;

	for (uint8 i = 0; i < OW_COUNT_ELEMENTS(PerfomanceMessages); i++)
	{
		if (PerfomanceMessages[i].what != PERF_DELIM)
		{
			m_UIMgr->RenderText(
				point,
				PerfomanceMessages[i].descr +
				string(": t[")
				+
				_Perfomance->GetTimer(PerfomanceMessages[i].what)
				+
				(
					PerfomanceMessages[i].showInc ? "], c["
					+
					_Perfomance->GetInc(PerfomanceMessages[i].what) + "]" : "]"
				), 
				COLOR_WHITE
			);
		}

		point.y += diff;
	}

	m_UIMgr->RenderText(point, "SUMMA: " + _Perfomance->Sum(), COLOR_WHITE);
}
