#pragma once

#include "WMO.h"
#include "WMO_Group_Instance.h"
#ifdef GAME_WMO_INCLUDE_WM2
#include "WMO_Doodad_Instance.h"
#endif
#include "WMO_Liquid_Instance.h"

class CWMO_Base_Instance : public SceneNode
{
public:
	CWMO_Base_Instance(std::weak_ptr<SceneNode> _parent, SmartWMOPtr _wmoObject);
	virtual ~CWMO_Base_Instance();

	// WMO_Base_Instance
	SmartWMOPtr getObject() { return m_Object; }
	void InitTransform();
	void EmptyTransformAndBounds();

	void AddGroupInstance(std::shared_ptr<CWMO_Group_Instance> _group) { m_GroupInstances.push_back(_group); }
	std::vector<std::shared_ptr<CWMO_Group_Instance>>& getGroupInstances() { return m_GroupInstances; }

	void AddOutdoorGroupInstance(std::shared_ptr<CWMO_Group_Instance> _group) { m_OutdoorGroupInstances.push_back(_group); }
	std::vector<std::shared_ptr<CWMO_Group_Instance>>& getGroupOutdoorInstances() { return m_OutdoorGroupInstances; }

	cmat4 getInvWorld() const { return m_InvWorld; }
	const vec3* getVerts() const { return m_ConvertedVerts.data(); }

	// ISceneNode
	//std::string getObjectInfo() const override { return "@WMO_Base@" + m_Object->getFilename(); }

	// IUpdatable
	void Update(double _time, double _dTime);

	// IRenderable3D
	bool PreRender3D();
	void Render3D();

protected:
	const SmartWMOPtr						m_Object;
	SWMO_Doodad_SetInfo						m_DoodadSetInfo;

	mat4									m_InvWorld;
	std::vector<vec3>							m_ConvertedVerts;
	
	std::vector<std::shared_ptr<CWMO_Group_Instance>>	m_GroupInstances;
	std::vector<std::shared_ptr<CWMO_Group_Instance>>	m_OutdoorGroupInstances;
};