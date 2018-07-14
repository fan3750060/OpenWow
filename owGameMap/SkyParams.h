#pragma once

class SkyParams
{
public:
	SkyParams();
	virtual ~SkyParams();

	SkyParams& operator+=(const SkyParams& _s);
	SkyParams& operator*=(float _weight);

	void Clear();

public:
	vec3                      m_Colors[LightColors::COUNT];
	float                     m_Fogs[LightFogs::COUNT];

	bool					  m_highlightSky;
	DBC_LightSkyboxRecord*	  m_SkyBox;
	float                     m_glow;
	float                     m_waterShallowAlpha;
	float                     m_waterDeepAlpha;
	float                     m_oceanShallowAlpha;
	float                     m_oceanDeepAlpha;


};