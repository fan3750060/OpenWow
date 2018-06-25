#pragma once

#include "Map_Headers.h"

// FORWARD BEGIN
class ADT;
// FORWARD END

class ADT_MCNK : public SceneNode
{
public:
	ADT_MCNK(ADT* _parentTile, IFile* _file);
	~ADT_MCNK();

	void RenderNormals();

	// ISceneNode
	string getObjectInfo() override { return "@ADT_MCNK@" + to_string(header.indexX) + ", " + to_string(header.indexY); }

	// ILoadableObject
	bool Load() override;
	bool Delete() override;

	// IRenderable
	bool PreRender3D() override;
	void Render3D() override;

public:
	const SmartPtr<IFile> m_File;
	const vector<SmartPtr<ADT_TextureInfo>>* m_ParentTextures;
	ADT_MCNK_Header header;

	ADT_MCNK_MCLY mcly[4];

	SmartTexturePtr m_DiffuseTextures[4];
	SmartTexturePtr m_SpecularTextures[4];


	SmartPtr<Liquid_Instance> m_LiquidInstance;

	SmartTexturePtr m_BlendRBGShadowATexture;

		
	SmartGeomPtr __geomDebugNormals;


	// Qulity
	SmartBufferPtr __ibHigh;
	uint16  m_IndexesCountHigh;
	SmartGeomPtr __geomHigh;

	SmartBufferPtr __ibDefault;
	uint16  m_IndexesCountDefault;
	SmartGeomPtr __geomDefault;

	CGroupQuality& m_QualitySettings;
};