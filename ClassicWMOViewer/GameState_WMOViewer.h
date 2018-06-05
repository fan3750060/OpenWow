#pragma once

class GameState_WMOViewer : public CGameState, CRenderable3DObject
{
public:
	GameState_WMOViewer();


	void CreateDebugGeom();
	void PlayAnim(uint16 _anim);

	// IGameState
	bool Init() override;
	void Destroy() override;
    bool Set() override;
    void Unset() override;

    // IUpdatable
    void Input(double _time, double _dTime) override;
	void Update(double _time, double _dTime) override;

	// IRenderable
	void PreRender3D(double _time, double _dTime) override;
	void Render3D() override;
	void PostRender3D() override;

	// IRenderableUI
	void RenderUI() override;

	// IInputListener
    void OnMouseMoved(cvec2 _mousePos) override;
    bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
    bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseWheel(int _yoffset) override { return false; }
    bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override { return false; }
	bool OnCharInput(uint32 _char) override { return false; }

private:
	SmartWMOPtr backgroundModel;

	float	animtime;
	uint32	globalTime;


    // Camera moving
    bool enableFreeCamera;
    bool cameraSprint;
    bool cameraSlow;
    vec2 lastMousePos;

	SmartGeomPtr m_DebugGeom;
};
