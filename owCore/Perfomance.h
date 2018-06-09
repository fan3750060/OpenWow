#pragma once

#define PERF_TIMERS_ENABLED
#define PERF_COUNTERS_ENABLED
#define PERF_TIMERS_COUNT 64
#define PERF_DELIM (static_cast<PerfomanceTimer>(PERF_TIMERS_COUNT + 1))

enum PerfomanceTimer : uint8
{
	// Map perfomance
	PERF_MAP = 0,
	PERF_MAP_CHUNK_GEOMETRY = 1,
	PERF_MAP_CHUNK_LIQUID = 2,
	PERF_MAP_CHUNK_MH20 = 3,
	PERF_MAP_LOWRESOLUTION = 4,

	// Global WMO
	PERF_MAP_MODELS_WMO_GLOBAL = 20,

	// WMOs
	PERF_MAP_MODELS_WMOs = 30,
	PERF_MAP_MODELS_WMOs_GEOMETRY = 31,
	PERF_MAP_MODELS_WMOs_DOODADS = 32,
	PERF_MAP_MODELS_WMOs_LIQUIDS = 33,

	// M2 (Map)
	PERF_MAP_MODELS_MDXs = 40,

	// Phases
	PERF_PHASE_INPUT = 50,
	PERF_PHASE_UPDATE = 51,
	PERF_PHASE_INPUT_AND_UPDATE = 52,
	PERF_PHASE_PRE3D = 53,
	PERF_PHASE_3D = 54,
	PERF_PHASE_POST3D = 55,

	//

	PERF_FRUSTRUM = 60
};

struct PerfomanceDebugOutput
{
	PerfomanceTimer what;
	const char* descr;
	bool showInc;
};

const PerfomanceDebugOutput PerfomanceMessages[] =
{
	{ PERF_MAP, "Map", false },

	{ PERF_DELIM, "", false },

	{ PERF_MAP_CHUNK_GEOMETRY, "Map Geom", true },
	{ PERF_MAP_CHUNK_LIQUID, "Map Liquid", true },
	{ PERF_MAP_CHUNK_MH20, "Map MH2O", true },
	{ PERF_MAP_LOWRESOLUTION, "Map Low-res", true },

	{ PERF_DELIM, "", false },

	// Global WMO
	{ PERF_MAP_MODELS_WMO_GLOBAL, "Map Global WMO", true },

	{ PERF_DELIM, "", false },

	// WMOs
	{ PERF_MAP_MODELS_WMOs, "Map WMOs", true },
	{ PERF_MAP_MODELS_WMOs_GEOMETRY, "Map WMOs Geom", true },
	{ PERF_MAP_MODELS_WMOs_DOODADS, "Map WMOs Doodads", true },
	{ PERF_MAP_MODELS_WMOs_LIQUIDS, "Map WMOs Liquids", true },

	{ PERF_DELIM, "", false },

	// M2 (Map)
	{ PERF_MAP_MODELS_MDXs, "Map MDXs", true },

	{ PERF_DELIM, "", false },

	// Phases
	{ PERF_PHASE_INPUT,		"Input",	true },
	{ PERF_PHASE_UPDATE,	"Up",	true },
	{ PERF_PHASE_PRE3D,		"MPRE3D",	true },
	{ PERF_PHASE_3D,		"M3D",		true },
	{ PERF_PHASE_POST3D,	"MPOST3D",	true }
};



class Perfomance
{
	CLASS_INSTANCE(Perfomance);
public:
	Perfomance();

	inline void FrameBegin()
	{
		for (uint8 i = 0; i < PERF_TIMERS_COUNT; i++)
		{
			m_Timer[i] = 0.0;
			m_TimerCurrent[i] = 0.0;
			m_DrawCount[i] = 0;
		}
	}

	// Timers

	inline void Start(PerfomanceTimer _timer)
	{
		m_TimerCurrent[_timer] = m_Engine->GetAdapter()->GetTime();
	}

	inline void Stop(PerfomanceTimer _timer)
	{
		m_TimerCurrent[_timer] = m_Engine->GetAdapter()->GetTime() - m_TimerCurrent[_timer];
		m_Timer[_timer] += m_TimerCurrent[_timer];
	}

	inline string GetTimer(PerfomanceTimer _timer)
	{
		return to_string(static_cast<uint32>(m_Timer[_timer] * 1000.0));
	}

	inline string GetInc(PerfomanceTimer _timer)
	{
		return to_string(m_DrawCount[_timer]);
	}

	inline string Sum()
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

	// Statistics

	inline void Inc(PerfomanceTimer _timer)
	{
		m_DrawCount[_timer] += 1;
	}

private:
	double m_Timer[PERF_TIMERS_COUNT];
	double m_TimerCurrent[PERF_TIMERS_COUNT];
	uint32 m_DrawCount[PERF_TIMERS_COUNT];
	IEngine* m_Engine;
};

#define _Perfomance Perfomance::instance()

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