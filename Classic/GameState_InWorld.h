#pragma once

class GameState_InWorld : public CGameState, CRenderable3DObject
{
public:
    bool Init() override;
    void Destroy() override;

    // IUpdatable
    void Input(double _time, double _dTime) override;
    void Update(double _time, double _dTime) override;

	// IRenderable
	void PreRender3D() override { }
	void Render3D() override;
	void PostRender3D() override {}

	// IRenderableUI
	void RenderUI() override;

    // IInputListener
    void OnMouseMoved(cvec2 _mousePos) override;
    bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
    bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
    bool OnMouseWheel(int _yoffset) override;
    bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
    bool OnKeyboardReleased(int _key, int _scancode, int _mods) override;
	bool OnCharInput(uint32 _char) override { return false; }

private:
    // Camera moving
    bool enableFreeCamera;
    bool cameraSprint;
    bool cameraSlow;
    vec2 lastMousePos;

    bool minimapActive;

private:
	WorldRender* m_WorldRender;
	CSceneManager* sceneManager;
};
