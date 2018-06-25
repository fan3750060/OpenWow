#pragma once

#define PERF_TIMERS_COUNT 100
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

	// Perfomance
	PERF_CULLING = 60,

	//

	PERF_GLOBAL = 70
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
	{ PERF_PHASE_INPUT,		"Input",		true },
	{ PERF_PHASE_UPDATE,	"Update",		true },
	{ PERF_PHASE_3D,		"Render",		true },

	{ PERF_DELIM, "", false },

	// Perfomance
	{ PERF_CULLING,			"Culling",		true },

	// Perfomance
	{ PERF_GLOBAL,			"Global",		true }
};

__interface IPerfomance
{
	void Start(PerfomanceTimer _timer);
	void Stop(PerfomanceTimer _timer);
	string GetTimer(PerfomanceTimer _timer);


	// Counts
	void Inc(PerfomanceTimer _timer);
	string GetInc(PerfomanceTimer _timer);
};