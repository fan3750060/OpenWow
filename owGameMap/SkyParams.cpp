#include "stdafx.h"

// General
#include "SkyParams.h"

SkyParams::SkyParams()
{
	Clear();
}

SkyParams::~SkyParams()
{
}

void SkyParams::Clear()
{
	for (uint8 i = 0; i < LightColors::COUNT; i++)
		m_Colors[i] = vec3();
	for (uint8 i = 0; i < LightFogs::COUNT; i++)
		m_Fogs[i] = 0.0f;

	m_glow = 0.0f;
	m_waterShallowAlpha = 0.0f;
	m_waterDeepAlpha = 0.0f;
	m_oceanShallowAlpha = 0.0f;
	m_oceanDeepAlpha = 0.0f;
}

SkyParams& SkyParams::operator+=(const SkyParams& _s)
{
	for (uint8 i = 0; i < LightColors::COUNT; i++)
		this->m_Colors[i] += _s.m_Colors[i];

	for (uint8 i = 0; i < LightFogs::COUNT; i++)
		this->m_Fogs[i] += _s.m_Fogs[i];

	this->m_glow += _s.m_glow;
	this->m_oceanShallowAlpha += _s.m_oceanShallowAlpha;
	this->m_oceanDeepAlpha += _s.m_oceanDeepAlpha;
	this->m_waterShallowAlpha += _s.m_waterShallowAlpha;
	this->m_waterDeepAlpha += _s.m_waterDeepAlpha;

	return *this;
}

SkyParams& SkyParams::operator*=(float _weight)
{
	for (uint8 i = 0; i < LightColors::COUNT; i++)
		this->m_Colors[i] *= _weight;

	for (uint8 i = 0; i < LightFogs::COUNT; i++)
		this->m_Fogs[i] *= _weight;

	this->m_glow *= _weight;
	this->m_oceanShallowAlpha *= _weight;
	this->m_oceanDeepAlpha *= _weight;
	this->m_waterShallowAlpha *= _weight;
	this->m_waterDeepAlpha *= _weight;

	return *this;
}