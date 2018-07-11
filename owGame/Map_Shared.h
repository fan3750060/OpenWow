#pragma once

class CMapShared
{
public:
	CMapShared();

	SharedBufferPtr BufferTextureCoordDetail;
	SharedBufferPtr BufferTextureCoordAlpha;

	static string getMapFolder(const DBC_MapRecord& _map);

	vector<uint16> GenarateHighMapArray(uint16 _holes = 0);
	vector<uint16> GenarateDefaultMapArray(uint16 _holes = 0);

private:
	vector<uint16>  m_HighMapStrip;
	vector<uint16>  m_DefaultMapStrip;
};

extern CMapShared* _MapShared;
