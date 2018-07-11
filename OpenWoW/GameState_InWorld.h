#pragma once

class GameState_InWorld : public CGameState, CRenderable3DObject
{
public:
	virtual ~GameState_InWorld();

    bool Init() override;
    void Destroy() override;
	void SwitchOnEnd() { m_Ended = true; _Render->getCamera()->EnableUpdate(); }

    // IUpdatable
	void Input(CInput* _input, double _time, double _dTime) override {}
    void Update(double _time, double _dTime) override;

	// IRenderable
	bool PreRender3D() override { return true; }
	void Render3D() override;
	void PostRender3D() override;

	// IRenderableUI
	void RenderUI() override;

    // IInputListener
    void OnMouseMoved(cvec2 _mousePos) override { }
    bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override { return false; }
    bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override { return false; }
    bool OnMouseWheel(int _yoffset) override { return false; }
    bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
    bool OnKeyboardReleased(int _key, int _scancode, int _mods) override { return false; }
	bool OnCharInput(uint32 _char) override { return false; }

private:
    bool minimapActive;

private:
	CSceneManager*			sceneManager;
	Camera*					m_TestCamera;
	R_RenderBuffer*			m_TestRenderBuffer;
	R_RenderBuffer*			m_TestRenderBufferFinal;

	CM2_Base_Instance*		m2_Camera;
	vec3					m_DefPos;
	float					m_Rot;
	bool					m_Ended;

	WorldRender*			m_WorldRender;
};
