#include "stdafx.h"

// General
#include "Input.h"

// Additional
#include "BindingController.h"

Input::Input()
{
    for (int i = 0; i < OW_KEYSCOUNT; i++)
    {
        keyState[i] = false;
    }

    for (int i = 0; i < OW_MOUSEBUTTONSCOUNT; i++)
    {
        mouseButtonState[i] = false;
    }

}

Input::~Input()
{}

//

void Input::MousePositionCallback(cvec2 _mousePos)
{
    mousePos = _mousePos;
    _Bindings->m_InputListenerObjectCollection->OnMouseMoved(mousePos);
}

void Input::MouseCallback(int button, int action, int mods)
{
    if (action == OW_PRESS)
    {
        mouseButtonState[button] = true;
		_Bindings->m_InputListenerObjectCollection->OnMouseButtonPressed(button, action, mousePos);
    }
    else if (action == OW_RELEASE)
    {
        mouseButtonState[button] = false;
		_Bindings->m_InputListenerObjectCollection->OnMouseButtonReleased(button, action, mousePos);
    }
}

void Input::MouseScrollCallback(int yoffset)
{
	_Bindings->m_InputListenerObjectCollection->OnMouseWheel(yoffset);
}

void Input::KeyboardCallback(int key, int scancode, int action, int mods)
{
    if (action == OW_PRESS || action == OW_REPEAT)
    {
        keyState[key] = true;
		_Bindings->m_InputListenerObjectCollection->OnKeyboardPressed(key, scancode, mods);
    }
    else if (action == OW_RELEASE)
    {
        keyState[key] = false;
		_Bindings->m_InputListenerObjectCollection->OnKeyboardReleased(key, scancode, mods);
    }
}

void Input::CharCallback(uint32 _char)
{
	_Bindings->m_InputListenerObjectCollection->OnCharInput(_char);
}

void Input::RegisterInputListenerObject(IInputListener* _object)
{
	_Bindings->m_InputListenerObjectCollection->RegisterObject(_object);
}
