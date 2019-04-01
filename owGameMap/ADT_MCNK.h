#pragma once

#include "Map_Headers.h"

// FORWARD BEGIN
class ADT;
class MapController;
// FORWARD END

class ADT_MCNK : public SceneNodeModel3D
{
    typedef SceneNodeModel3D base;
public:
	ADT_MCNK(std::weak_ptr<MapController> _mapController, std::weak_ptr<ADT> _parentTile, const std::string& _fileName, const ADT_MCIN& _mcin);
	virtual ~ADT_MCNK();

	// SceneNodeModel3D
	void UpdateLocalTransform() override;
	bool Accept(IVisitor& visitor) override;

	// ILoadable
	bool PreLoad() override;
	bool Load() override;
	bool Delete() override;
	uint32 getPriority() const override { return 1; };

public:
	std::string m_FileName;
	std::shared_ptr<IFile> m_File;
	const ADT_MCIN mcin;
	ADT_MCNK_Header header;

	ADT_MCNK_MCLY mcly[4];
	std::shared_ptr<Texture> m_DiffuseTextures[4];
	std::shared_ptr<Texture> m_SpecularTextures[4];

	std::shared_ptr<Liquid_Instance> m_LiquidInstance;

	std::shared_ptr<Texture> m_BlendRBGShadowATexture;

	// Qulity
	std::shared_ptr<IMesh> __geomHigh;
	std::shared_ptr<IMesh> __geomDefault;

private: // PARENT
	const std::weak_ptr<MapController>	m_MapController;
	const std::weak_ptr<ADT>			m_ParentADT;
	CGroupQuality&						m_QualitySettings;
};
