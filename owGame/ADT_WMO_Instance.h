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
	ADT_WMO_Instance(WMO* _wmoObject, ADT_MODF& _placementInfo);
	~ADT_WMO_Instance();

	inline WMO* GetWMO() { return m_Object; }

	// IRenderable3D
	void PreRender3D(double _time, double _dTime) override;
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