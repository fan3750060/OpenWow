#pragma once

#include "InputKeys.h"
#include "InputListenerCollection.h"

class Input
{
public:
    Input();
    ~Input();

    vec2 GetMouse() { return mousePos; }
    bool IsKeyPressed(int key) { return keyState[key]; }
    bool IsMousePressed(int button) { return mouseButtonState[button]; }

	// GLFW Connector
    void MousePositionCallback(cvec2 _mousePos);
    void MouseCallback(int button, int action, int mods);
    void MouseScrollCallback(int yoffset);
    void KeyboardCallback(int key, int scancode, int action, int mods);
    void CharCallback(uint32 _char);

	//

	void RegisterInputListenerObject(InputListenerObject* _object);

private:
	bool keyState[OW_KEYSCOUNT];
    bool mouseButtonState[OW_MOUSEBUTTONSCOUNT];
    vec2 mousePos;
};
