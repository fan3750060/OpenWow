#pragma once

class Map_Shared
{
	CLASS_INSTANCE(Map_Shared);
public:
	void Init();

	//

	const vec2& GetTextureCoordDetail() { return m_TCDetailMap[0]; }
	const vec2& GetTextureCoordAlpha() { return m_TCAlphaMap[0]; }

	static vector<uint16> GenarateDefaultMapArray(uint16 _holes = 0);
	static vector<uint16> GenarateLowResMapArray(uint16 _holes = 0);

private:
	vec2            m_TCDetailMap[C_MapBufferSize];
	vec2            m_TCAlphaMap[C_MapBufferSize];
	vector<uint16>  m_DefaultMapStrip;
	vector<uint16>  m_LowResMapStrip;
};

#define _Map_Shared Map_Shared::instance()