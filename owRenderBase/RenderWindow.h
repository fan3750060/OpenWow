#pragma once

#include "RenderEvents.h"

class Camera;
class Buffer;
class StructuredBuffer;
class Scene;
class Mesh;
class Shader;
class Texture;
class SamplerState;
class Rect;
class Material;
class RenderTarget;

class RenderWindow : public Object
{
public:
	typedef Object base;

	virtual void ShowWindow(); // Show this window if it is hidden.
	virtual void HideWindow(); // Hide the window. The window will not be destroyed and can be  shown again using the ShowWindow() function.
	virtual void CloseWindow(); // Destroy and close the window.

	int GetWindowWidth() const;
	int GetWindowHeight() const;
	bool IsVSync() const;
	HWND GetHWND() const;
	virtual void SetWindowName(cstring _name);
	cstring GetWindowName() const;

	// Present the back buffers
	virtual void Present() = 0;

	// Get the render target of this render window.
	virtual std::shared_ptr<RenderTarget> GetRenderTarget() = 0;

	// Invoked when the window is initialized.
	Event				Initialize;
	Event				Terminate;

	// Window is closing
	WindowCloseEvent    Close;

	// Window events
	Event           InputFocus;// Window gets input focus
	Event           InputBlur; // Window loses input focus
	Event           Minimize; // Window is minimized.
	Event           Restore;// Window is restored.

	// Invoked when the window contents should be repainted.
	// (Part of the window is now "exposed".
	Event           Expose;

	// Update event is called when the application
	// will be updated before rendering.
	// This this callback to update your game logic.
	UpdateEvent			Update;

	// Invoked when the window needs to be redrawn.
	RenderEvent         PreRender;
	RenderEvent         Render;
	RenderEvent         PostRender;

	// Keyboard events
	KeyboardEvent       KeyPressed;
	KeyboardEvent       KeyReleased;
	// Window gained keyboard focus
	Event               KeyboardFocus;
	// Window lost keyboard focus.
	Event               KeyboardBlur;

	// The mouse was moved over the window
	MouseMotionEvent    MouseMoved;
	// A mouse button was pressed over the window
	MouseButtonEvent    MouseButtonPressed;
	// A mouse button was released over the window.
	MouseButtonEvent    MouseButtonReleased;
	// The mouse wheel was changed
	MouseWheelEvent     MouseWheel;
	// The mouse left the client area.
	Event               MouseLeave;
	// The window has gained the mouse focus.
	Event               MouseFocus;
	// The window has lost mouse focus.
	Event               MouseBlur;

	// The window was resized.
	ResizeEvent         Resize;

	virtual void OnInitialize(EventArgs& e);

	virtual void OnUpdate(UpdateEventArgs& e);
	virtual void OnPreRender(RenderEventArgs& e);
	virtual void OnRender(RenderEventArgs& e);
	virtual void OnPostRender(RenderEventArgs& e);

	// The application is terminating.
	virtual void OnTerminate(EventArgs& e);

protected:
	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Only the application can create windows.
	RenderWindow(cstring windowName, int windowWidth, int windowHeight, HWND _hwnd, bool vSync = false);
	virtual ~RenderWindow();

	// The application window has received focus
	virtual void OnInputFocus(EventArgs& e);
	// The application window has lost focus
	virtual void OnInputBlur(EventArgs& e);


	// The application window has been minimized
	virtual void OnMinimize(EventArgs& e);
	// The application window has been restored
	virtual void OnRestore(EventArgs& e);

	// The application window has be resized
	virtual void OnResize(ResizeEventArgs& e);
	// The window contents should be repainted
	virtual void OnExpose(EventArgs& e);

	// The user requested to exit the application
	virtual void OnClose(WindowCloseEventArgs& e);


	// A keyboard key was pressed
	virtual void OnKeyPressed(KeyEventArgs& e);
	// A keyboard key was released
	virtual void OnKeyReleased(KeyEventArgs& e);
	// Window gained keyboard focus
	virtual void OnKeyboardFocus(EventArgs& e);
	// Window lost keyboard focus
	virtual void OnKeyboardBlur(EventArgs& e);

	// The mouse was moved
	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	// A button on the mouse was pressed
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e);
	// A button on the mouse was released
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);
	// The mouse wheel was moved.
	virtual void OnMouseWheel(MouseWheelEventArgs& e);
	// The mouse left the client are of the window.
	virtual void OnMouseLeave(EventArgs& e);
	// The application window has received mouse focus
	virtual void OnMouseFocus(EventArgs& e);
	// The application window has lost mouse focus
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

	// Keep track of the event connections so that I can disconnect them if the
	// window is closed.
	// Also automatically closes any registered events when the window is destroyed.
	//Event::ScopedConnections m_EventConnections;

};
