#pragma once

#include "InputListener.h"

class InputListenerObjectCollection
{
public:
    bool RegisterObject(InputListenerObject* _uiObject);
    void UnregisterObject(InputListenerObject* _uiObject);

    //

    void OnMouseMoved(cvec2 _mousePos);
    void OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos);
    void OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos);
    void OnMouseWheel(int _yoffset);
    void OnKeyboardPressed(int _key, int _scancode, int _mods);
    void OnKeyboardReleased(int _key, int _scancode, int _mods);
    void OnCharInput(uint32 _char);

private:
    vector<InputListenerObject*> m_Objects;
};