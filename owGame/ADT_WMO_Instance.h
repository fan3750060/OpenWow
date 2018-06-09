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

	WMO* GetWMO() { return m_Object; }

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;

private:
	SmartWMOPtr	m_Object;
	uint32		m_UniqueId;
	uint16		m_DoodadSetIndex;

public:	// Static
	static void reset();
private:
	static std::set<uint32> m_AlreadyDraw;
};