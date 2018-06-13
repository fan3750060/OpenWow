#pragma once

class 
	__declspec(uuid("CDC1D2DA-0258-43B7-9561-A15C31F3D7A2"))
	CGroupQuality : public ISettingGroup, public IInputListener
{
public:
	CGroupQuality();
	~CGroupQuality();

	uint16 Texture_Sampler;

	// Drawing
	bool draw_mcnk;
	bool draw_mcnk_low;
	bool draw_map_wmo;
	bool draw_wmo_doodads;
	bool draw_map_m2;
	bool draw_water;
	bool draw_wmo_water;

	bool WMO_MOCV;
	bool WMO_AmbColor;

	bool drawfog;
	bool timeEnable;

	// ISettingGroup
	void InitDefault() override;

	// IInputListener
	void OnMouseMoved(cvec2 _mousePos) override {}
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override { return false; }
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override { return false; }
	bool OnMouseWheel(int _yoffset) override { return false; }
	bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override { return false; }
	bool OnCharInput(uint32 _char) override { return false; }
};