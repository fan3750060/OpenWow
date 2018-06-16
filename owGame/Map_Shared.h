#pragma once

class Map_Shared
{
public:
	static void Init();

	//

	static SmartBufferPtr BufferTextureCoordDetail;
	static SmartBufferPtr BufferTextureCoordAlpha;

	static vector<uint16> GenarateHighMapArray(uint16 _holes = 0);
	static vector<uint16> GenarateDefaultMapArray(uint16 _holes = 0);

private:
	static vector<uint16>  m_HighMapStrip;
	static vector<uint16>  m_DefaultMapStrip;
};