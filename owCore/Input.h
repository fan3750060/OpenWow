#pragma once

#include "InputKeys.h"

class CInput
{
public:
    CInput();

    vec2 GetMouse() { return m_MousePos; }
    bool IsKeyPressed(int key) { return m_KeyState[key]; }
    bool IsMousePressed(int button) { return m_MouseButtonState[button]; }

	void setOnFileDropperCallback(Function_WA<const char*>* _onDrop);

	// GLFW Connector
    void MousePositionCallback(cvec2 _mousePos);
    void MouseCallback(int button, int action, int mods);
    void MouseScrollCallback(int yoffset);
    void KeyboardCallback(int key, int scancode, int action, int mods);
    void CharCallback(uint32 _char);
	void DropFilesCallback(int _count, const char** _fnames);

private:
	vec2						m_MousePos;
	bool						m_KeyState[OW_KEYSCOUNT];
    bool						m_MouseButtonState[OW_MOUSEBUTTONSCOUNT];
	Function_WA<const char*>*	m_OnFileDropped;
};
