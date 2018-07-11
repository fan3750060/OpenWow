#include "stdafx.h"

// General
#include "Sky.h"

Sky::Sky()
{}

Sky::Sky(DBC_LightRecord data) :
	m_LightRecord(data)
{
    m_Position.x = data.Get_PositionX() / skymul;
    m_Position.y = data.Get_PositionY() / skymul;
    m_Position.z = data.Get_PositionZ() / skymul;

    m_Range.min = data.Get_RadiusInner() / skymul;
    m_Range.max = data.Get_RadiusOuter() / skymul;

    m_IsGlobalSky = (m_Position.x == 0.0f && m_Position.y == 0.0f && m_Position.z == 0.0f);
    if (m_IsGlobalSky)
    {
        Log::Error("Sky [%d] is GLOBAL!!!!", data.Get_ID());
    }

    LoadParams(LightParamsNames::ParamsClear);
}

Sky::~Sky()
{
}

void Sky::LoadParams(LightParamsNames _param)
{
    for (uint32 i = 0; i < LightColors::LIGHT_COLORS_COUNT; i++)
    {
        m_IntBand_Colors[i].clear();
    }

    for (uint32 i = 0; i < LightFogs::LIGHT_FOGS_COUNT; i++)
    {
        m_FloatBand_Fogs[i].clear();
    }

    uint32 paramSet = m_LightRecord.Get_LightParamsID()->Get_ID();

    //-----------------------------------------------
    //-- Color params
    //-----------------------------------------------

    for (uint32 i = 0; i < LightColors::LIGHT_COLORS_COUNT; i++)
    {
        DBC_LightIntBandRecord* lightColorsRecord = DBC_LightIntBand[paramSet * LightColors::LIGHT_COLORS_COUNT - (LightColors::LIGHT_COLORS_COUNT - 1) + i];
        if (lightColorsRecord == nullptr)
        {
            Log::Error("Color record for [%d] not fornd. SkyID [%d]", i, m_LightRecord.Get_ID());
            continue;
        }

        // Read time & color value
        for (uint32 l = 0; l < lightColorsRecord->Get_Count(); l++)
        {
            m_IntBand_Colors[i].push_back(SkyParam_Color(lightColorsRecord->Get_Times(l), lightColorsRecord->Get_Values(l)));
        }
    }

    //-----------------------------------------------
    //-- Fog, Sun, Clouds param
    //-----------------------------------------------

    for (uint32 i = 0; i < LightFogs::LIGHT_FOGS_COUNT; i++)
    {
        DBC_LightFloatBandRecord* lightFogRecord = DBC_LightFloatBand[paramSet * LightFogs::LIGHT_FOGS_COUNT - (LightFogs::LIGHT_FOGS_COUNT - 1) + i];
        if (lightFogRecord == nullptr)
        {
            Log::Error("Fog record for [%d] not fornd. SkyID [%d]", i, m_LightRecord.Get_ID());
            continue;
        }

        // Read time & fog param
        for (uint32 l = 0; l < lightFogRecord->Get_Count(); l++)
        {
            float param = lightFogRecord->Get_Values(l);
            if (i == LightFogs::LIGHT_FOG_DISTANCE)
            {
                param /= 36.0f;
            }
            m_FloatBand_Fogs[i].push_back(SkyParam_Fog(lightFogRecord->Get_Times(l), param));
        }
    }

    //-----------------------------------------------
    //-- LightParams
    //-----------------------------------------------
    DBC_LightParamsRecord* lightParamRecord = DBC_LightParams[paramSet];
    if (lightParamRecord != nullptr)
    {
        m_highlightSky = lightParamRecord->Get_HighlightSky();
        m_glow = lightParamRecord->Get_Glow();
        m_waterShallowAlpha = lightParamRecord->Get_WaterShallowAlpha();
        m_waterDeepAlpha = lightParamRecord->Get_WaterDeepAlpha();
        m_oceanShallowAlpha = lightParamRecord->Get_OceanShallowAlpha();
        m_oceanDeepAlpha = lightParamRecord->Get_OceanDeepAlpha();

        if (m_lightSkyboxID() != nullptr)
        {
            Log::Error("Skybox name = [%s]", m_lightSkyboxID()->Get_Filename());
        }
    }
    else
    {
        Log::Error("LightParams record for LightID [%d] not found!", m_LightRecord.Get_ID());
    }
}



SkyParams& Sky::GetByTime(uint32 _time)
{
    for (uint8 i = 0; i < LightColors::LIGHT_COLORS_COUNT; i++)
    {
        m_InterpolatedColors[i] = GetByTimeTemplate(m_IntBand_Colors, i, _time);
    }
    for (uint8 i = 0; i < LightFogs::LIGHT_FOGS_COUNT; i++)
    {
        m_InterpolatedFogs[i] = GetByTimeTemplate(m_FloatBand_Fogs, i, _time);
    }

    return *this;
}

SkyParams::~SkyParams()
{
}
