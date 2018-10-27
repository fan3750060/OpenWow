#pragma once

class GameState_WMOViewer : public CGameState, CRenderable3DObject
{
public:
	void CreateDebugGeom();
	void OpenFile(const char* _fname);

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
    void OnMouseMoved(cvec2 _mousePos) override { return; }
    bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override { return false; }
    bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override { return false; }
	bool OnMouseWheel(int _yoffset) override { return false; }
    bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override { return false; }
	bool OnCharInput(uint32 _char) override { return false; }


private:
	CSceneManager*			sceneManager;
	Camera*					m_TestCamera;
	R_RenderBuffer*			m_TestRenderBuffer;
	R_RenderBuffer*			m_TestRenderBufferFinal;

private:
	UniquePtr<CWMO_Base_Instance>	backgroundModel;
	std::shared_ptr<IMesh>					m_DebugGeom;



	
};
