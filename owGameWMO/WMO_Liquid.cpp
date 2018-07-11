#include "stdafx.h"

// General
#include "WMO_Liquid.h"

CWMO_Liquid::CWMO_Liquid(uint32 _x, uint32 _y) :
	Liquid(_x, _y)
{}

void CWMO_Liquid::CreateFromWMO(IFile* f, WMO_Part_Material* _material, const DBC_LiquidTypeRecord* _liquidType, bool _indoor)
{
	ydir = -1.0f; // Magic for WMO
	initGeometry(_liquidType, f);

	/*if (_indoor)
	{
		m_WaterColorLight = fromRGB(_material->GetDiffuseColor());
		m_WaterColorDark = m_WaterColorLight;
	}
	else
	{
		m_WaterColorLight = _World->EnvM()->m_SkyManager->GetColor(LIGHT_COLOR_RIVER_LIGHT);
		m_WaterColorDark = _World->EnvM()->m_SkyManager->GetColor(LIGHT_COLOR_RIVER_DARK);
	}*/
}