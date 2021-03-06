#include "stdafx.h"

// Include
#include "SkyManager.h"

// General
#include "Sky.h"

const float skymul = 36.0f;

Sky::Sky() :
	m_LightRecord(nullptr)
{}

Sky::Sky(const DBC_LightRecord& _record) :
	m_LightRecord(&_record)
{
	m_Position.x = _record.Get_PositionX() / skymul;
	m_Position.y = _record.Get_PositionY() / skymul;
	m_Position.z = _record.Get_PositionZ() / skymul;
	m_Range.min = _record.Get_RadiusInner() / skymul;
	m_Range.max = _record.Get_RadiusOuter() / skymul;

	m_IsGlobalSky = (m_Position.x == 0.0f && m_Position.y == 0.0f && m_Position.z == 0.0f);
	if (m_IsGlobalSky)
	{
		Log::Error("Sky [%d] is GLOBAL!!!!", _record.Get_ID());
	}

	LoadParams(LightParamsNames::ParamsClear);
}

Sky::~Sky()
{}


class SkyParam_Color : public Sky::SkyParam<vec3>
{
public:
	SkyParam_Color(uint32 _time, uint32 _color) : SkyParam<vec3>(_time, fromRGB(_color)) {}
};
class SkyParam_Fog : public Sky::SkyParam<float>
{
public:
	SkyParam_Fog(uint32 _time, float _param) : SkyParam<float>(_time, _param) {}
};

void Sky::LoadParams(LightParamsNames _param)
{
	for (uint32 i = 0; i < LightColors::COUNT; i++)
	{
		m_IntBand_Colors[i].clear();
	}
	for (uint32 i = 0; i < LightFogs::COUNT; i++)
	{
		m_FloatBand_Fogs[i].clear();
	}

	std::shared_ptr<const DBC_LightParamsRecord> paramRecord = DBC_LightParams[m_LightRecord->Get_LightParams(_param)];
	assert1(paramRecord != nullptr);
	uint32 paramSet = paramRecord->Get_ID();

	//-- LightParams
	m_Params.m_highlightSky = paramRecord->Get_HighlightSky();
	m_Params.m_SkyBox = paramRecord->Get_LightSkyboxID();
	m_Params.m_glow = paramRecord->Get_Glow();
	m_Params.m_waterShallowAlpha = paramRecord->Get_WaterShallowAlpha();
	m_Params.m_waterDeepAlpha = paramRecord->Get_WaterDeepAlpha();
	m_Params.m_oceanShallowAlpha = paramRecord->Get_OceanShallowAlpha();
	m_Params.m_oceanDeepAlpha = paramRecord->Get_OceanDeepAlpha();

	if (m_Params.m_SkyBox != nullptr)
	{
		Log::Green("!!!Skybox name = [%s]", m_Params.m_SkyBox->Get_Filename());
	}

	//-- Color params
	for (uint32 i = 0; i < LightColors::COUNT; i++)
	{
		std::shared_ptr<const DBC_LightIntBandRecord> lightColorsRecord = DBC_LightIntBand[paramSet * LightColors::COUNT - (LightColors::COUNT - 1) + i];
		assert1(lightColorsRecord != nullptr);
		for (uint32 l = 0; l < lightColorsRecord->Get_Count(); l++)
		{
			// Read time & color value
			m_IntBand_Colors[i].push_back(SkyParam_Color(lightColorsRecord->Get_Times(l), lightColorsRecord->Get_Values(l)));
		}
	}

	//-- Fog, Sun, Clouds param
	for (uint32 i = 0; i < LightFogs::COUNT; i++)
	{
		std::shared_ptr<DBC_LightFloatBandRecord> lightFogRecord = DBC_LightFloatBand[paramSet * LightFogs::COUNT - (LightFogs::COUNT - 1) + i];
		assert1(lightFogRecord != nullptr);
		for (uint32 l = 0; l < lightFogRecord->Get_Count(); l++)
		{
			// Read time & fog param
			float param = lightFogRecord->Get_Values(l);
			if (i == LightFogs::LIGHT_FOG_DISTANCE)	param /= skymul;
			m_FloatBand_Fogs[i].push_back(SkyParam_Fog(lightFogRecord->Get_Times(l), param));
		}
	}
}



SkyParams& Sky::Interpolate(uint32 _time)
{
	for (uint8 i = 0; i < LightColors::COUNT; i++)
	{
		m_Params.m_Colors[i] = GetByTimeTemplate(m_IntBand_Colors, i, _time);
	}
	for (uint8 i = 0; i < LightFogs::COUNT; i++)
	{
		m_Params.m_Fogs[i] = GetByTimeTemplate(m_FloatBand_Fogs, i, _time);
	}

	return m_Params;
}


