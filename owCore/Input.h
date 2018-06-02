#pragma once

#include "InputKeys.h"

class Input
{
public:
    Input();

    vec2 GetMouse() { return m_MousePos; }
    bool IsKeyPressed(int key) { return m_KeyState[key]; }
    bool IsMousePressed(int button) { return m_MouseButtonState[button]; }

	// GLFW Connector
    void MousePositionCallback(cvec2 _mousePos);
    void MouseCallback(int button, int action, int mods);
    void MouseScrollCallback(int yoffset);
    void KeyboardCallback(int key, int scancode, int action, int mods);
    void CharCallback(uint32 _char);

private:
	vec2 m_MousePos;
	bool m_KeyState[OW_KEYSCOUNT];
    bool m_MouseButtonState[OW_MOUSEBUTTONSCOUNT];
};
