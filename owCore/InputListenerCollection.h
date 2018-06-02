#pragma once

class CInputListenerObjectCollection : public IInputListener
{
public:
    bool RegisterObject(IInputListener* _object);
    void UnregisterObject(IInputListener* _object);

    // IInputListener
    void OnMouseMoved(cvec2 _mousePos) override;
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseWheel(int _yoffset) override;
	bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override;
	bool OnCharInput(uint32 _char) override;

private:
    vector<IInputListener*> m_Objects;
};