#pragma once

struct ADT_MODF
{
	uint32 nameIndex;
	uint32 uniqueId;
	vec3 position;
	vec3 rotation;
	CAaBox boundingBox;
	uint16 flags;
	uint16 doodadSetIndex;
	uint16 nameSet;
	uint16 unk0; 
};

class ADT_WMO_Instance : public SceneNode
{
public:
	ADT_WMO_Instance(SceneNode* _parent, WMO* _wmoObject, ADT_MODF& _placementInfo);
	~ADT_WMO_Instance();

	void AddDoodad(WMO_MODD_Instance* _doodadInstance) { m_Doodads.push_back(_doodadInstance); }
	void AddLiquid(Liquid_Instance* _liquidInstance) { m_Liquids.push_back(_liquidInstance); }

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;

private:
	const SmartWMOPtr	m_Object;
	uint32				m_UniqueId;
	uint16				m_DoodadSetIndex;

	vector<SmartPtr<WMO_MODD_Instance>> m_Doodads;
	vector<SmartPtr<Liquid_Instance>> m_Liquids;

public:	// Static
	static void reset();
private:
	static std::set<uint32> m_AlreadyDraw;
};