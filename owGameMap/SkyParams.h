#pragma once

struct SkyParams
{
	SkyParams();

	const SkyParams& operator+=(const SkyParams& _s);
	const SkyParams& operator*=(float _weight);

	void Clear();

	vec3                      m_Colors[LightColors::COUNT];
	float                     m_Fogs[LightFogs::COUNT];

	bool					  m_highlightSky;
	const DBC_LightSkyboxRecord*	  m_SkyBox;
	float                     m_glow;
	float                     m_waterShallowAlpha;
	float                     m_waterDeepAlpha;
	float                     m_oceanShallowAlpha;
	float                     m_oceanDeepAlpha;
};