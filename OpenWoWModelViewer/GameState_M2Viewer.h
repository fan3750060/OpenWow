#pragma once

#include "CM2_Viewer_Instance.h"

class GameState_M2Viewer : public CGameState, CRenderable3DObject
{
public:
	GameState_M2Viewer();


	void CreateDebugGeom();
	void PlayAnim(uint16 _anim);
	void InfoAnim();
	void OpenFile(const char* _fname);

	// IGameState
	bool Init() override;
	void Destroy() override;
    bool Set() override;
    void Unset() override;

    // IUpdatable
    void Input(CInput* _input, double _time, double _dTime) override {}
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
	UniquePtr<CM2_Base_Instance> m2_Model;
	SmartGeomPtr m_DebugGeom;
	Character* m_Char;
};
