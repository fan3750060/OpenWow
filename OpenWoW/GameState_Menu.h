#pragma once

enum Commands
{
	CMD_NONE,
	CMD_SELECT
};

class GameState_Menu : public CGameState, CRenderable3DObject
{
public:
	virtual ~GameState_Menu();

	void OnBtn(DBC_MapRecord _e);
	bool LoadWorld(vec3 _pos);

	// IGameState
	bool Init() override;
	void Destroy() override;
    bool Set() override;
    void Unset() override;

    // IUpdatable
	void Input(CInput* _input, double _time, double _dTime) override {};
	void Update(double _time, double _dTime) override;

	// IRenderable
	bool PreRender3D() override;
	void Render3D() override;
	void PostRender3D() override;

	// IRenderableUI
	void RenderUI() override;

	// IInputListener
    void OnMouseMoved(cvec2 _mousePos) override { }
    bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
    bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override { return false; }
	bool OnMouseWheel(int _yoffset) override { return false; }
    bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override { return false; }
	bool OnCharInput(uint32 _char) override { return false; }

private:
    UIElement* m_MinimapUI;
	UIElement* m_LoadingScreenUI;

	vector<SharedPtr<UIWowButon>> m_Buttons;

	Commands cmd;

	void randBackground();
	SharedPtr<CM2_Base_Instance> m_BackgroudModel;
};
