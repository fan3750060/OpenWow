#pragma once

#include "WMO_Headers.h"

class CWMO_Part_Portal
{
public:
	CWMO_Part_Portal(const WMO* _parentWMO, const WMO_PortalDef& _proto);

	void Render();

private:
	const WMO*				m_ParentWMO;
	const WMO_PortalDef		m_Proto;

	SmartGeomPtr			m_Geom;
};