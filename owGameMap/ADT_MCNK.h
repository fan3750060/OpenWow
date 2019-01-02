#pragma once

#include "Map_Headers.h"

// FORWARD BEGIN
class ADT;
class MapController;
// FORWARD END

class ADT_MCNK : public SceneNode, public ILoadable
{
public:
	ADT_MCNK(std::weak_ptr<MapController> _mapController, std::weak_ptr<ADT> _parentTile, std::shared_ptr<IFile> _file);
	virtual ~ADT_MCNK();

	// SceneNode
	void CalculateLocalTransform(bool _isRotationQuat = false) override { assert1(false); }

	// ILoadable
	bool Load() override;
	bool Delete() override;
	void setLoaded() {}
	bool isLoaded() const { return true; } // TODO FIXME

	void Accept(IVisitor& visitor) override;

public:
	std::shared_ptr<IFile> m_File;
	ADT_MCNK_Header header;

	ADT_MCNK_MCLY mcly[4];
	std::shared_ptr<Texture> m_DiffuseTextures[4];
	std::shared_ptr<Texture> m_SpecularTextures[4];

	std::shared_ptr<Liquid_Instance> m_LiquidInstance;

	std::shared_ptr<Texture> m_BlendRBGShadowATexture;

	// Qulity
	std::shared_ptr<Buffer> __ibHigh;
	std::shared_ptr<IMesh> __geomHigh;

	std::shared_ptr<Buffer> __ibDefault;
	std::shared_ptr<IMesh> __geomDefault;

private: // PARENT
	const std::weak_ptr<MapController>	m_MapController;
	const std::weak_ptr<ADT>			m_ParentADT;
	CGroupQuality&						m_QualitySettings;
};
