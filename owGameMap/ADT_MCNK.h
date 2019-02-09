#pragma once

#include "Map_Headers.h"

// FORWARD BEGIN
class ADT;
class MapController;
// FORWARD END

class ADT_MCNK : public SceneNode3D
{
public:
	ADT_MCNK(std::weak_ptr<MapController> _mapController, std::weak_ptr<ADT> _parentTile
#if (VERSION == VERSION_WotLK)
		, cstring _fileName
		, const ADT_MCIN& _mcin
#endif
	);
	virtual ~ADT_MCNK();

	// SceneNode3D
	void UpdateLocalTransform() override;
	bool Accept(IVisitor& visitor) override;

	// ILoadable
	bool PreLoad() override;
	bool Load() override;
	bool Delete() override;
	uint32 getPriority() const override { return 1; };

	// VERSION_Cata split files
	void Load_MCNK_ADT(std::shared_ptr<IFile> _file, uint32 size);
	void Load_MCNK_OBJ(std::shared_ptr<IFile> _file, uint32 size);
	void Load_MCNK_TEX(std::shared_ptr<IFile> _file, uint32 size);

public:
	std::string m_FileName;
#if (VERSION == VERSION_WotLK)
	std::shared_ptr<IFile> m_File;
	const ADT_MCIN mcin;
#endif
	ADT_MCNK_Header m_Header;

	std::shared_ptr<Texture> m_DiffuseTextures[4];
	std::shared_ptr<Texture> m_SpecularTextures[4];

	std::shared_ptr<Liquid_Instance> m_LiquidInstance;

	std::shared_ptr<IMesh> m_Mesh;

private: // PARENT
	const std::weak_ptr<MapController>	m_MapController;
	const std::weak_ptr<ADT>			m_ParentADT;
	CGroupQuality&						m_QualitySettings;
};
