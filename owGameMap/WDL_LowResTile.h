#pragma once

// FORWARD BEGIN
class MapController;
// FORWARD END

class CWDL_LowResTile : public SceneNode
{
public:
	CWDL_LowResTile(MapController* _parent, uint32 _indexX, uint32 _indexZ, R_GeometryInfo* _geom);

	// ISceneNode
	string getObjectInfo() const override { return "@WDL_LOWRES@"; }

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

private:
	const int				m_IndexX, m_IndexZ;

	RenderState				m_State;
	SharedGeomPtr			m_Geom;

private: // PARENT
	const MapController*	m_MapController;
};