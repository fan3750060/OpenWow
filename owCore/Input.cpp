#include "stdafx.h"

// General
#include "Input.h"

CInput::CInput() : 
	m_OnFileDropped(nullptr)
{
	for (int i = 0; i < OW_KEYSCOUNT; i++) m_KeyState[i] = false;
	for (int i = 0; i < OW_MOUSEBUTTONSCOUNT; i++) m_MouseButtonState[i] = false;
}

//

void CInput::setOnFileDropperCallback(Function_WA<const char*>* _onDrop)
{
	if (m_OnFileDropped != nullptr)
	{
		delete m_OnFileDropped;
	}

	_ASSERT(_onDrop);
	m_OnFileDropped = _onDrop;
}

void CInput::MousePositionCallback(cvec2 _mousePos)
{
	m_MousePos = _mousePos;
	_Bindings->m_InputListenerObjectCollection->OnMouseMoved(m_MousePos);
}

void CInput::MouseCallback(int button, int action, int mods)
{
	if (action == OW_PRESS)
	{
		m_MouseButtonState[button] = true;
		_Bindings->m_InputListenerObjectCollection->OnMouseButtonPressed(button, action, m_MousePos);
	}
	else if (action == OW_RELEASE)
	{
		m_MouseButtonState[button] = false;
		_Bindings->m_InputListenerObjectCollection->OnMouseButtonReleased(button, action, m_MousePos);
	}
}

void CInput::MouseScrollCallback(int yoffset)
{
	_Bindings->m_InputListenerObjectCollection->OnMouseWheel(yoffset);
}

void CInput::KeyboardCallback(int key, int scancode, int action, int mods)
{
	if (action == OW_PRESS || action == OW_REPEAT)
	{
		m_KeyState[key] = true;
		_Bindings->m_InputListenerObjectCollection->OnKeyboardPressed(key, scancode, mods);
	}
	else if (action == OW_RELEASE)
	{
		m_KeyState[key] = false;
		_Bindings->m_InputListenerObjectCollection->OnKeyboardReleased(key, scancode, mods);
	}
}

void CInput::CharCallback(uint32 _char)
{
	_Bindings->m_InputListenerObjectCollection->OnCharInput(_char);
}

void CInput::DropFilesCallback(int _count, const char** _fnames)
{
	_ASSERT(_count > 0);
	m_OnFileDropped->operator()(_fnames[0]);
}
