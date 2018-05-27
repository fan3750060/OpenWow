#pragma once

class GameState_InWorld : public GameState
{
public:
    GameState_InWorld(Engine* _Engine) : GameState(_Engine) {}

    bool Init() override;
    void Destroy() override;

    //

    void Input(double t, double dt) override;
    void Update(double t, double dt) override;
    void Render(double t, double dt);
    void RenderUI() override;

    void RenderUIDebug();

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
};
