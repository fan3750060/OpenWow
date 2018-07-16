#pragma once

#include "Map_Headers.h"

// FORWARD BEGIN
class ADT;
class MapController;
// FORWARD END

class ADT_MCNK : public SceneNode, public ILoadable
{
public:
	ADT_MCNK(MapController* _mapController, ADT* _parentTile, IFile* _file);
	virtual ~ADT_MCNK();

	void RenderNormals();

	// ISceneNode
	string getObjectInfo() const override { return "@ADT_MCNK@" + to_string(header.indexX) + ", " + to_string(header.indexY); }
	void CalculateMatrix(bool _isRotationQuat = false) override { fail1(); };

	// ILoadable
	bool Load() override;
	bool Delete() override;
	void setLoaded() {}
	bool isLoaded() const { return true; } // TODO FIXME

	// IRenderable
	bool PreRender3D() override;
	void Render3D() override;

	static void BindUniforms(Technique* _techique);

public:
	SharedPtr<IFile> m_File;
	ADT_MCNK_Header header;

	ADT_MCNK_MCLY mcly[4];
	SharedTexturePtr m_DiffuseTextures[4];
	SharedTexturePtr m_SpecularTextures[4];

	SharedPtr<Liquid_Instance> m_LiquidInstance;

	SharedTexturePtr m_BlendRBGShadowATexture;


	SharedGeomPtr __geomDebugNormals;

	// Qulity
	RenderDrawCall m_HighCall;
	RenderState m_StateHigh;
	SharedBufferPtr __ibHigh;
	uint16  m_IndexesCountHigh;
	SharedGeomPtr __geomHigh;

	RenderDrawCall m_DefaultCall;
	RenderState m_StateDefault;
	SharedBufferPtr __ibDefault;
	uint16  m_IndexesCountDefault;
	SharedGeomPtr __geomDefault;

private: // PARENT
	const MapController*				m_MapController;
	const ADT*							m_ParentADT;
	CGroupQuality&						m_QualitySettings;
};