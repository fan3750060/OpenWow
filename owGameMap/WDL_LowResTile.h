#pragma once

// FORWARD BEGIN
class MapController;
// FORWARD END

class CWDL_LowResTile : public SceneNode
{
public:
	CWDL_LowResTile(std::weak_ptr<MapController> _parent, uint32 _indexX, uint32 _indexZ, SharedMeshPtr _geom);

	// SceneNode
	void Render(RenderEventArgs& renderEventArgs) override;

private:
	const int				           m_IndexX, m_IndexZ;
	SharedMeshPtr			           m_Geom;

private: // PARENT
	const std::weak_ptr<MapController> m_MapController;
};