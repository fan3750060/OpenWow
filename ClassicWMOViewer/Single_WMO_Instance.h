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

class Single_WMO_Instance : public SceneNode
{
public:
	Single_WMO_Instance(SceneNode* _parent, WMO* _wmoObject);
	~Single_WMO_Instance();

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;

private:
	const SmartWMOPtr	m_Object;
};