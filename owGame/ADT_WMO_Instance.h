#pragma once

struct ADT_MODF
{
	uint32 nameIndex;		// Ref to MWID
	uint32 uniqueId;		// this ID should be unique for all ADTs currently loaded. Best, they are unique for the whole map.
	vec3 position;		
	vec3 rotation;
	CAaBox boundingBox;		// position plus the transformed wmo bounding box. used for defining if they are rendered as well as collision.
	struct Flags
	{
		uint16 Destroyable : 1;
		uint16 : 15;
	} flags;
	uint16 doodadSetIndex;	// which WMO doodad set is used.
	uint16 nameSet;			// which WMO name set is used. Used for renaming goldshire inn to northshire inn while using the same model.
	uint16 scale;			// Legion+
};

class ADT_WMO_Instance : public SceneNode
{
public:
	ADT_WMO_Instance(SceneNode* _parent, WMO* _wmoObject, ADT_MODF& _placementInfo);
	~ADT_WMO_Instance();

	// ISceneNode
	string getObjectInfo() override { return m_Object->getFilename(); }

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	void PreRender3D() override;
	void Render3D() override;

private:
	const SmartWMOPtr						m_Object;
	uint32									m_UniqueId;
	WMO_Doodad_SetInfo						m_DoodadSetInfo;
	SmartPtr<CWMO_InstanceController>		m_InstanceController;

	const CGroupQuality&					m_QualitySettings;
	const CGroupDistances&					m_DistancesSettings;

public:	// Static
	static void reset();
private:
	static std::set<uint32> m_AlreadyDraw;
};