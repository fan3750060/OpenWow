#pragma once

#include "WMO.h"
#include "WMO_Group_Instance.h"
#include "WMO_Doodad_Instance.h"
#include "WMO_Liquid_Instance.h"

class CWMO_Base_Instance : public SceneNode
{
public:
	CWMO_Base_Instance(SceneNode* _parent, WMO* _wmoObject);
	virtual ~CWMO_Base_Instance();

	// WMO_Base_Instance
	WMO* getObject() { return m_Object; }
	void InitTransform();
	void EmptyTransformAndBounds();

	void AddGroupInstance(CWMO_Group_Instance* _group) { m_GroupInstances.push_back(_group); }
	vector<CWMO_Group_Instance*>& getGroupInstances() { return m_GroupInstances; }

	void AddOutdoorGroupInstance(CWMO_Group_Instance* _group) { m_OutdoorGroupInstances.push_back(_group); }
	vector<CWMO_Group_Instance*>& getGroupOutdoorInstances() { return m_OutdoorGroupInstances; }

	cmat4 getInvWorld() const { return m_InvWorld; }
	const vec3* getVerts() const { return m_ConvertedVerts.data(); }

	// ISceneNode
	string getObjectInfo() override { return "@WMO_Base@" + m_Object->getFilename(); }

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

protected:
	const SmartWMOPtr						m_Object;
	SWMO_Doodad_SetInfo						m_DoodadSetInfo;

	mat4									m_InvWorld;
	vector<vec3>							m_ConvertedVerts;
	
	vector<CWMO_Group_Instance*>			m_GroupInstances;
	vector<CWMO_Group_Instance*>			m_OutdoorGroupInstances;

	const CGroupQuality&					m_QualitySettings;
};