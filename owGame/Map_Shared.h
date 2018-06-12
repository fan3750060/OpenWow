#pragma once

class Map_Shared
{
	CLASS_INSTANCE(Map_Shared);
public:
	void Init();

	//

	SmartBufferPtr BufferTextureCoordDetail;
	SmartBufferPtr BufferTextureCoordAlpha;

	static vector<uint16> GenarateDefaultMapArray(uint16 _holes = 0);
	static vector<uint16> GenarateLowResMapArray(uint16 _holes = 0);

private:
	vector<uint16>  m_DefaultMapStrip;
	vector<uint16>  m_LowResMapStrip;
};

#define _Map_Shared Map_Shared::instance()