#pragma once

#include "UIElement.h"

class UIInput : public UIElement
{
    typedef UIElement base;
public:
    UIInput();

    void Init(cvec2 _position, cvec2 _size, InputMode _inputMode = MODE_ANY);

    //

    void SetInputString(cstring _inputString) { m_Text = _inputString; }
    void SetInputMaxLenght(int _inputStringMaxLenght) { inputStringMaxLenght = _inputStringMaxLenght; }
    void SetInputMode(InputMode _inputMode) { inputMode = _inputMode; }

    string Data() const { return m_Text; }

    //

    virtual void OnRenderUI();

	// IInputrListener
    bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
    bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
    bool OnCharInput(uint32 _char) override;

private:
    int inputStringMaxLenght;

    InputMode inputMode;
};
