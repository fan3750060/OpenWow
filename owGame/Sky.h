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

template<typename T>
class SkyParam
{
public:
    SkyParam(uint32 _time, T _param)
    {
        time = _time;
        param = _param;
    }

    uint32 time;
    T param;
};

class SkyParam_Color : public SkyParam<vec3>
{
public:
    SkyParam_Color(uint32 _time, uint32 _color) : SkyParam<vec3>(_time, fromRGB(_color))
    {}
};

class SkyParam_Fog : public SkyParam<float>
{
public:
    SkyParam_Fog(uint32 _time, float _param) : SkyParam<float>(_time, _param)
    {}
};

//

class SkyParams
{
public:
	virtual ~SkyParams();

    void Clear()
    {
        for (uint8 i = 0; i < LightColors::LIGHT_COLORS_COUNT; i++)
        {
            m_InterpolatedColors[i] = vec3();
        }
        for (uint8 i = 0; i < LightFogs::LIGHT_FOGS_COUNT; i++)
        {
            m_InterpolatedFogs[i] = 0.0f;
        }

        m_glow = 0.0f;
        m_waterShallowAlpha = 0.0f;
        m_waterDeepAlpha = 0.0f;
        m_oceanShallowAlpha = 0.0f;
        m_oceanDeepAlpha = 0.0f;
    }

    SkyParams& operator+=(const SkyParams& _s)
    {
        for (uint8 i = 0; i < LightColors::LIGHT_COLORS_COUNT; i++)
            this->m_InterpolatedColors[i] += _s.m_InterpolatedColors[i];

        for (uint8 i = 0; i < LightFogs::LIGHT_FOGS_COUNT; i++)
            this->m_InterpolatedFogs[i] += _s.m_InterpolatedFogs[i];

        this->m_glow += _s.m_glow;
        this->m_oceanShallowAlpha += _s.m_oceanShallowAlpha;
        this->m_oceanDeepAlpha += _s.m_oceanDeepAlpha;
        this->m_waterShallowAlpha += _s.m_waterShallowAlpha;
        this->m_waterDeepAlpha += _s.m_waterDeepAlpha;

        return *this;
    }

    SkyParams& operator*=(float _weight)
    {
        for (uint8 i = 0; i < LightColors::LIGHT_COLORS_COUNT; i++)
            this->m_InterpolatedColors[i] *= _weight;

        for (uint8 i = 0; i < LightFogs::LIGHT_FOGS_COUNT; i++)
            this->m_InterpolatedFogs[i] *= _weight;

        this->m_glow *= _weight;
        this->m_oceanShallowAlpha *= _weight;
        this->m_oceanDeepAlpha *= _weight;
        this->m_waterShallowAlpha *= _weight;
        this->m_waterDeepAlpha *= _weight;

        return *this;
    }

public: // Params
    vec3                      m_InterpolatedColors[LightColors::LIGHT_COLORS_COUNT];
    float                     m_InterpolatedFogs[LightFogs::LIGHT_FOGS_COUNT];
    float                     m_glow;
    float                     m_waterShallowAlpha;
    float                     m_waterDeepAlpha;
    float                     m_oceanShallowAlpha;
    float                     m_oceanDeepAlpha;
};


class Sky : public CRefItem, public SkyParams
{
public:
    Sky();
    Sky(DBC_LightRecord data);
	virtual ~Sky();

    void LoadParams(LightParamsNames _param);

    SkyParams& GetByTime(uint32 _time);

private:
    template<typename T>
    T GetByTimeTemplate(vector<SkyParam<T>>* _array, uint32 _paramNum, uint32 _time) const;

public:
    vec3             m_Position;
    CRange           m_Range;
    float            m_Wight;
    bool             m_IsGlobalSky;
    bool             m_highlightSky;
    FOREIGN_KEY_ID(uint32, DBC_LightSkybox, m_lightSkyboxID);

private:
    vector<SkyParam<vec3>>    m_IntBand_Colors[LightColors::LIGHT_COLORS_COUNT];
    vector<SkyParam<float>>   m_FloatBand_Fogs[LightFogs::LIGHT_FOGS_COUNT];

private:
    DBC_LightRecord m_LightRecord;
    const float skymul = 36.0f;
};

#include "Sky.inl"




