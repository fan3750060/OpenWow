#include "stdafx.h"

// General
#include "GroupQuality.h"

CGroupQuality::CGroupQuality()
{
	_Bindings->RegisterInputListener(this);
}

CGroupQuality::~CGroupQuality()
{
	_Bindings->UnregisterInputListener(this);
}

void CGroupQuality::InitDefault()
{
	Terrain_LowDetail = false;
	Terrain_MCCV = true;
	Terrain_MCLV = true;
	Texture_Sampler = R_SamplerState::SS_ANISO16;

	draw_map_chunk = true;
	draw_map_wmo = true;
	draw_map_wmo_doodads = true;
	draw_map_mdx = true;
	draw_water = true;

	drawfog = false;
	timeEnable = false;
}

bool CGroupQuality::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_KP_1)
	{
		SwitchBool(draw_map_chunk);
		return true;
	}
	if (_key == OW_KEY_KP_2)
	{
		SwitchBool(draw_map_wmo);
		return true;
	}
	if (_key == OW_KEY_KP_3)
	{
		SwitchBool(draw_map_wmo_doodads);
		return true;
	}

	if (_key == OW_KEY_KP_4)
	{
		SwitchBool(draw_map_mdx);
		return true;
	}

	if (_key == OW_KEY_F1)
	{
		Texture_Sampler = R_SamplerState::SS_FILTER_POINT;
		return true;
	}

	if (_key == OW_KEY_F2)
	{
		Texture_Sampler = R_SamplerState::SS_FILTER_BILINEAR;
		return true;
	}

	if (_key == OW_KEY_F3)
	{
		Texture_Sampler = R_SamplerState::SS_FILTER_TRILINEAR;
		return true;
	}

	if (_key == OW_KEY_F6)
	{
		Texture_Sampler = R_SamplerState::SS_ANISO1;
		return true;
	}

	if (_key == OW_KEY_F7)
	{
		Texture_Sampler = R_SamplerState::SS_ANISO2;
		return true;
	}

	if (_key == OW_KEY_F8)
	{
		Texture_Sampler = R_SamplerState::SS_ANISO4;
		return true;
	}

	if (_key == OW_KEY_F9)
	{
		Texture_Sampler = R_SamplerState::SS_ANISO8;
		return true;
	}

	if (_key == OW_KEY_F10)
	{
		Texture_Sampler = R_SamplerState::SS_ANISO16;
		return true;
	}

	if (_key == OW_KEY_C)
	{
		SwitchBool(Terrain_MCCV);
		return true;
	}

	if (_key == OW_KEY_V)
	{
		SwitchBool(Terrain_MCLV);
		return true;
	}

	if (_key == OW_KEY_F)
	{
		SwitchBool(drawfog);
		return true;
	}

	if (_key == OW_KEY_T)
	{
		SwitchBool(timeEnable);
		return true;
	}

	return false;
}
