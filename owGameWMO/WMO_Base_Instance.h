#pragma once

#include "WMO.h"
#include "WMO_InstanceController.h"
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
	void InitDefault();
	void AddDoodadInstance(CWMO_Doodad_Instance* _doodad) { m_WMODoodads.push_back(_doodad); }
	void AddWMOLiquidInstance(CWMO_Liquid_Instance* _doodad) { m_WMOLiquids.push_back(_doodad); }

	// ISceneNode
	string getObjectInfo() override { return "@WMO_Base@" + m_Object->getFilename(); }

	// IUpdatable
	void Update(double _time, double _dTime) override;

	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;

protected:
	const SmartWMOPtr						m_Object;
	CWMO_InstanceController*				m_InstanceController;
	WMO_Doodad_SetInfo						m_DoodadSetInfo;

	vector<SmartPtr<CWMO_Doodad_Instance>>	m_WMODoodads;
	vector<SmartPtr<CWMO_Liquid_Instance>>	m_WMOLiquids;

	const CGroupQuality&					m_QualitySettings;
};