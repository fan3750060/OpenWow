#pragma once

struct MinimapData_Map
{
	uint8		X_Coord;
	uint8		Y_Coord;
	string		filename;
};

struct MinimapDir
{
	MinimapDir()
	{
	}

	void Load();
	void Render(cvec3 _pos);

	string						name;
	vector<MinimapData_Map*>	data;
	SharedTexturePtr			textures[C_TilesInMap][C_TilesInMap];
};

class CMinimapProvider
{
public:
	CMinimapProvider();
	~CMinimapProvider();

	//--

	void Init();
	MinimapDir* getMinimap(string _name);

private:
	std::shared_ptr<IFile> m_File;

	vector<MinimapDir*> m_Minimaps;
	const CGroupVideo& m_GroupVideo;
};