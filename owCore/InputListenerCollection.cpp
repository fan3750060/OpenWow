#include "stdafx.h"

// General
#include "InputListenerCollection.h"

CInputListenerObjectCollection::~CInputListenerObjectCollection()
{
}

bool CInputListenerObjectCollection::RegisterObject(IInputListener* _Object, uint8 _priority)
{
    m_Objects.push_back(CInputListener(_Object, _priority));
	std::sort(m_Objects.begin(), m_Objects.end(), CInputListenerObjectCompare());

    return true;
}

void CInputListenerObjectCollection::UnregisterObject(IInputListener* _Object)
{
	for (auto& it  = m_Objects.begin(); it != m_Objects.end(); ++it)
	{
		if ((it)->getInputListener() == _Object)
		{
			m_Objects.erase(it);
			break;
		}
	}
}

//

void CInputListenerObjectCollection::OnMouseMoved(cvec2 _mousePos)
{
    for (auto& it : m_Objects)
    {
        it.OnMouseMoved(_mousePos);
    }
}

bool CInputListenerObjectCollection::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
{
    for (auto& it : m_Objects)
    {
        if (it.OnMouseButtonPressed(_button, _mods, _mousePos))
        {
            return true;
        }
    }

	return false;
}

bool CInputListenerObjectCollection::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
{
    for (auto& it : m_Objects)
    {
        if (it.OnMouseButtonReleased(_button, _mods, _mousePos))
        {
            return true;
        }
    }

	return false;
}

bool CInputListenerObjectCollection::OnMouseWheel(int _yoffset)
{
    for (auto& it : m_Objects)
    {
        if (it.OnMouseWheel(_yoffset))
        {
            return true;
        }
    }

	return false;
}

bool CInputListenerObjectCollection::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
    for (auto& it : m_Objects)
    {
        if (it.OnKeyboardPressed(_key, _scancode, _mods))
        {
            return true;
        }
    }

	return false;
}

bool CInputListenerObjectCollection::OnKeyboardReleased(int _key, int _scancode, int _mods)
{
    for (auto& it : m_Objects)
    {
        if (it.OnKeyboardReleased(_key, _scancode, _mods))
        {
            return true;
        }
    }

	return false;
}

bool CInputListenerObjectCollection::OnCharInput(uint32 _char)
{
    for (auto& it : m_Objects)
    {
        if (it.OnCharInput(_char))
        {
            return true;
        }
    }

	return false;
}
