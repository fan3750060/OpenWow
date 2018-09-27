#pragma once

#include "Map_Headers.h"

// FORWARD BEGIN
class ADT;
class MapController;
// FORWARD END

class ADT_MCNK : public SceneNode, public ILoadable
{
public:
	ADT_MCNK(std::weak_ptr<MapController> _mapController, std::weak_ptr<ADT> _parentTile, IFile* _file);
	virtual ~ADT_MCNK();

	void RenderNormals();

	// ISceneNode
	//string getObjectInfo() const override { return "@ADT_MCNK@" + to_string(header.indexX) + ", " + to_string(header.indexY); }
	//void CalculateMatrix(bool _isRotationQuat = false) override { fail1(); };

	// ILoadable
	bool Load() override;
	bool Delete() override;
	void setLoaded() {}
	bool isLoaded() const { return true; } // TODO FIXME

	// IRenderable
	bool PreRender3D();
	void Render3D();

	//static void BindUniforms(Technique* _techique);

public:
	IFile* m_File;
	ADT_MCNK_Header header;

	ADT_MCNK_MCLY mcly[4];
	SharedTexturePtr m_DiffuseTextures[4];
	SharedTexturePtr m_SpecularTextures[4];

	std::shared_ptr<Liquid_Instance> m_LiquidInstance;

	SharedTexturePtr m_BlendRBGShadowATexture;


	SharedMeshPtr __geomDebugNormals;

	// Qulity
	SharedBufferPtr __ibHigh;
	uint16  m_IndexesCountHigh;
	SharedMeshPtr __geomHigh;

	SharedBufferPtr __ibDefault;
	uint16  m_IndexesCountDefault;
	SharedMeshPtr __geomDefault;

private: // PARENT
	const std::weak_ptr<MapController>	m_MapController;
	const std::weak_ptr<ADT>			m_ParentADT;
	CGroupQuality&						m_QualitySettings;
};
