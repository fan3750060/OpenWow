#pragma once

#include "RenderEvents.h"

class Camera;
class IBuffer;
class StructuredBuffer;
class Scene;
class IMesh;
class Shader;
class Texture;
class SamplerState;
class Rect;
class Material;
class IRenderTarget;

class RenderWindow : public Object
{
	typedef Object base;
public:

	void ShowWindow(); // Show this window if it is hidden.
	void HideWindow(); // Hide the window. The window will not be destroyed and can be  shown again using the ShowWindow() function.
	void CloseWindow(); // Destroy and close the window.

	void SetMousePosition(vec2 _position);

	int GetWindowWidth() const;
	int GetWindowHeight() const;
	bool IsVSync() const;
	HWND GetHWND() const;
	virtual void SetWindowName(cstring _name);
	cstring GetWindowName() const;

	virtual void Present() = 0;

	// Get the render target of this render window.
	virtual std::shared_ptr<IRenderTarget> GetRenderTarget() = 0;

	
	// Update event is called when the application will be updated before rendering.
	// This this callback to update your game logic.
	UpdateEvent			Update;

	// Invoked when the window needs to be redrawn.
	RenderEvent         PreRender;
	RenderEvent         Render;
	RenderEvent         PostRender;
	RenderUIEvent       RenderUI;

	// Invoked when the window is initialized.
	Event				Initialize;
	Event				Terminate;

	// Window events
	Event				InputFocus;// Window gets input focus
	Event				InputBlur; // Window loses input focus

	Event				Minimize; // Window is minimized.
	Event				Restore;// Window is restored.

	ResizeEvent         Resize;
	Event				Expose;

	// Window is closing
	WindowCloseEvent    Close;

	// Keyboard events
	KeyboardEvent       KeyPressed;
	KeyboardEvent       KeyReleased;
	Event               KeyboardFocus;
	Event               KeyboardBlur;

	// The mouse events
	MouseMotionEvent    MouseMoved;
	MouseButtonEvent    MouseButtonPressed;
	MouseButtonEvent    MouseButtonReleased;
	MouseWheelEvent     MouseWheel;
	Event               MouseLeave;
	Event               MouseFocus;
	Event               MouseBlur;

	

	virtual void OnInitialize(EventArgs& e);

	virtual void OnUpdate(UpdateEventArgs& e);
	virtual void OnPreRender(RenderEventArgs& e);
	virtual void OnRender(RenderEventArgs& e);
	virtual void OnPostRender(RenderEventArgs& e);
	virtual void OnRenderUI(RenderUIEventArgs& e);

	virtual void OnTerminate(EventArgs& e);

protected:
	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	RenderWindow(cstring windowName, int windowWidth, int windowHeight, HWND _hwnd, bool vSync = false);
	virtual ~RenderWindow();

	virtual void OnInputFocus(EventArgs& e);
	virtual void OnInputBlur(EventArgs& e);

	virtual void OnMinimize(EventArgs& e);
	virtual void OnRestore(EventArgs& e);

	virtual void OnResize(ResizeEventArgs& e);
	virtual void OnExpose(EventArgs& e);

	virtual void OnClose(WindowCloseEventArgs& e);

	// Input

	virtual void OnKeyPressed(KeyEventArgs& e);
	virtual void OnKeyReleased(KeyEventArgs& e);
	virtual void OnKeyboardFocus(EventArgs& e);
	virtual void OnKeyboardBlur(EventArgs& e);

	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual void OnMouseWheel(MouseWheelEventArgs& e);
	virtual void OnMouseLeave(EventArgs& e);
	virtual void OnMouseFocus(EventArgs& e);
	virtual void OnMouseBlur(EventArgs& e);

private:
	int m_iWindowWidth;
	int m_iWindowHeight;
	bool m_vSync;
	HWND m_hWindow;
	std::string m_sWindowName;

	// Used to compute relative mouse motion in this window.
	glm::ivec2 m_PreviousMousePosition;

	// This is true when the mouse is inside the window's client rect.
	bool m_bInClientRect;

	// This is set to true when the window receives keyboard focus.
	bool m_bHasKeyboardFocus;
};
