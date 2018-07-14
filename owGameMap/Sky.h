#pragma once

enum LightParamsNames
{
    ParamsClear, 	// Used in clear weather.
    ParamsClearWat, // Used in clear weather while being underwater.
    ParamsStorm,    // Used in rainy / snowy / sandstormy weather.
    ParamsStormWat, // Used in rainy / snowy / sandstormy weather while being underwater.
    ParamsDeath,	// Only 4 and in newer ones 3 are used as value here(with some exceptions).Changing this seems to have no effect in 3.3.5a(is death light setting hardcoded ? )
    ParamsUnk1, 	// Following 3 Params are used only in WotLK records, and not very commonly.Perhaps used in different phases.
    ParamsUnk2,
    ParamsUnk3
};

#include "SkyParams.h"

// FORWARD BEGIN
class SkyManager;
// FORWARD END

class Sky : public CRefItem
{
	friend class SkyManager;
public: // TYPES
	template<typename T>
	struct SkyParam
	{
		SkyParam(uint32 _time, T _value) : time(_time), value(_value) {}
		uint32 time;
		T value;
	};

public:
    Sky();
    Sky(const DBC_LightRecord& data);
	virtual ~Sky();

    void LoadParams(LightParamsNames _param);

    SkyParams& Interpolate(uint32 _time);

private:
	const DBC_LightRecord*	m_LightRecord;

    vec3					m_Position;
    CRange					m_Range;

    float					m_Wight;
    bool					m_IsGlobalSky;

	SkyParams				m_Params;
    vector<SkyParam<vec3>>  m_IntBand_Colors[LightColors::COUNT];
    vector<SkyParam<float>> m_FloatBand_Fogs[LightFogs::COUNT];
};

#include "Sky.inl"




