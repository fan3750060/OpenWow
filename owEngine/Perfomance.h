#pragma once

#define PERF_TIMERS_ENABLED
#define PERF_COUNTERS_ENABLED

class Perfomance : public IPerfomance
{
public:
	Perfomance();

	void FrameBegin();
	void FrameEnd();

	// Timers

	void Start(PerfomanceTimer _timer) override
	{
		m_TimerCurrent[_timer] = _Render->getAdapter()->GetTime();
	}

	void Stop(PerfomanceTimer _timer) override
	{
		m_TimerCurrent[_timer] = _Render->getAdapter()->GetTime() - m_TimerCurrent[_timer];
		m_Timer[_timer] += m_TimerCurrent[_timer];
	}

	string GetTimer(PerfomanceTimer _timer) override
	{
		return to_string(static_cast<uint32>(m_Timer[_timer] * 1000.0));
	}

	// Counts

	void Inc(PerfomanceTimer _timer) override
	{
		m_DrawCount[_timer] += 1;
	}

	string GetInc(PerfomanceTimer _timer) override
	{
		return to_string(m_DrawCount[_timer]);
	}

	//--

	inline string Sum();
	void Render(cvec2 _pos);

private:
	double m_Timer[PERF_TIMERS_COUNT];
	double m_TimerCurrent[PERF_TIMERS_COUNT];
	uint32 m_DrawCount[PERF_TIMERS_COUNT];

	IUIMgr* m_UIMgr;
};

extern Perfomance* _Perfomance;

// Macros

#ifdef PERF_TIMERS_ENABLED
#define PERF_START(timer) _Perfomance->Start(timer);
#define PERF_STOP(timer) _Perfomance->Stop(timer);
#else
#define PERF_START(timer)
#define PERF_STOP(timer)
#endif

#ifdef PERF_COUNTERS_ENABLED
#define PERF_INC(timer) _Perfomance->Inc(timer);
#else
#define PERF_INC(timer)
#endif