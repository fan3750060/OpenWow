#pragma once

enum Commands
{
	CMD_NONE,
	CMD_SELECT
};

class GameState_Menu : public CGameState, CRenderable3DObject
{
public:
	void OnBtn(DBC_MapRecord* _e);
	bool LoadWorld(vec3 _pos);

	// IGameState
	bool Init() override;
	void Destroy() override;
    bool Set() override;
    void Unset() override;

    // IUpdatable
    void Input(double _time, double _dTime) override;
	void Update(double _time, double _dTime) override;

	// IRenderable
	void PreRender3D() override;
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
    R_Texture* m_MinimapTexture;
    UIElement* m_MinimapUI;

	Commands cmd;

	SmartM2Ptr backgroundModel;
	float mt;

	void randBackground();



    // Camera moving
    bool enableFreeCamera;
    bool cameraSprint;
    bool cameraSlow;
    vec2 lastMousePos;
};
