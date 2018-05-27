#pragma once

enum Commands
{
	CMD_NONE,
	CMD_SELECT
};

class GameState_Menu : public GameState
{
public:
	GameState_Menu(Engine* _Engine) : GameState(_Engine) {}

	bool Init() override;
	void Destroy() override;

    //

    bool Set() override;
    void Unset() override;

    //

    void Input(double t, double dt) override;
	void Update(double t, double dt) override;
	void Render(double t, double dt);
	void RenderUI() override;

	//

    void OnBtn(DBC_MapRecord* _e);

	bool LoadWorld(vec3 _pos);

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

	MDX *backgroundModel;
	float mt;

	void randBackground();



    // Camera moving
    bool enableFreeCamera;
    bool cameraSprint;
    bool cameraSlow;
    vec2 lastMousePos;
};
