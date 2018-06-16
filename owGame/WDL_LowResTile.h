#pragma once

// FORWARD BEGIN
class MapController;
// FORWARD END

class CWDL_LowResTile : public SceneNode
{
public:
	CWDL_LowResTile(SceneNode* _parent, uint32 _indexX, uint32 _indexZ, R_GeometryInfo* _geom);

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;

private:
	const MapController*	m_MapController;
	const int				m_IndexX, m_IndexZ;
	SmartGeomPtr			m_Geom;
};