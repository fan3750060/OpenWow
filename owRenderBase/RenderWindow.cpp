#include "stdafx.h"

// General
#include "RenderWindow.h"

// Additional
#include "Camera.h"

RenderWindow::RenderWindow(cstring windowName, int windowWidth, int windowHeight, HWND _hwnd, bool vSync)
	: m_sWindowName(windowName)
	, m_iWindowWidth(windowWidth)
	, m_iWindowHeight(windowHeight)
	, m_hWindow(_hwnd)
	, m_vSync(vSync)
	, m_PreviousMousePosition(0, 0)
	, m_bInClientRect(false)
	, m_bHasKeyboardFocus(false)
{

}

RenderWindow::~RenderWindow()
{
}

void RenderWindow::ShowWindow()
{
	::ShowWindow(m_hWindow, SW_SHOWDEFAULT);
	::BringWindowToTop(m_hWindow);
}

void RenderWindow::HideWindow()
{
	::ShowWindow(m_hWindow, SW_HIDE);
}

void RenderWindow::CloseWindow()
{
	::DestroyWindow(m_hWindow);
}

void RenderWindow::SetMousePosition(vec2 _position)
{
	RECT rc;
	GetClientRect(m_hWindow, &rc); // get client coords
	ClientToScreen(m_hWindow, reinterpret_cast<POINT*>(&rc.left)); // convert top-left
	ClientToScreen(m_hWindow, reinterpret_cast<POINT*>(&rc.right)); // convert bottom-right

	SetCursorPos(rc.left + _position.x, rc.top + _position.y);
}



int RenderWindow::GetWindowWidth() const
{
	return m_iWindowWidth;
}

int RenderWindow::GetWindowHeight() const
{
	return m_iWindowHeight;
}

bool RenderWindow::IsVSync() const
{
	return m_vSync;
}

HWND RenderWindow::GetHWND() const
{
	return m_hWindow;
}

void RenderWindow::SetWindowName(cstring _name)
{
	m_sWindowName = _name;

	::SetWindowTextA(m_hWindow, m_sWindowName.c_str());
}

cstring RenderWindow::GetWindowName() const
{
	return m_sWindowName;
}

void RenderWindow::OnInitialize(EventArgs& e)
{
	Initialize(e);
}

void RenderWindow::OnTerminate(EventArgs& e)
{
	Terminate(e);
}
// The window has received focus
void RenderWindow::OnInputFocus(EventArgs& e)
{
	InputFocus(e);
}

// The RenderWindow window has lost focus
void RenderWindow::OnInputBlur(EventArgs& e)
{
	InputBlur(e);
}

// The RenderWindow window has been minimized
void RenderWindow::OnMinimize(EventArgs& e)
{
	Minimize(e);
}

// The RenderWindow window has been restored
void RenderWindow::OnRestore(EventArgs& e)
{
	Restore(e);
}

// The RenderWindow window has be resized
void RenderWindow::OnResize(ResizeEventArgs& e)
{
	m_iWindowWidth = e.Width;
	m_iWindowHeight = e.Height;

	Resize(e);
}

// The window contents should be repainted
void RenderWindow::OnExpose(EventArgs& e)
{
	Expose(e);
}

void RenderWindow::OnUpdate(UpdateEventArgs& e)
{
	Update(e);
}

// The prepare the window for redraw (update shader parameters)
void RenderWindow::OnPreRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState);
	PreRender(renderArgs);
}

// The window should be redrawn.
void RenderWindow::OnRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState);
	Render(renderArgs);
}

// Handle any post-drawing events (like GUI)
void RenderWindow::OnPostRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState);
	PostRender(renderArgs);
}

// A keyboard key was pressed
void RenderWindow::OnKeyPressed(KeyEventArgs& e)
{
	KeyPressed(e);
}

// A keyboard key was released
void RenderWindow::OnKeyReleased(KeyEventArgs& e)
{
	KeyReleased(e);
}

// Window gained keyboard focus
void RenderWindow::OnKeyboardFocus(EventArgs& e)
{
	m_bHasKeyboardFocus = true;
	KeyboardFocus(e);
}

// Window lost keyboard focus
void RenderWindow::OnKeyboardBlur(EventArgs& e)
{
	m_bHasKeyboardFocus = false;
	KeyboardBlur(e);
}

// The mouse was moved
void RenderWindow::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (!m_bInClientRect)
	{
		m_PreviousMousePosition = glm::ivec2(e.X, e.Y);
		m_bInClientRect = true;
	}

	e.RelX = e.X - m_PreviousMousePosition.x;
	e.RelY = e.Y - m_PreviousMousePosition.y;

	m_PreviousMousePosition = glm::ivec2(e.X, e.Y);

	MouseMoved(e);
}

// A button on the mouse was pressed
void RenderWindow::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	MouseButtonPressed(e);
}

// A button on the mouse was released
void RenderWindow::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	MouseButtonReleased(e);
}

void RenderWindow::OnMouseWheel(MouseWheelEventArgs& e)
{
	MouseWheel(e);
}

void RenderWindow::OnMouseLeave(EventArgs& e)
{
	m_bInClientRect = false;
	MouseLeave(e);
}

// The window has received mouse focus
void RenderWindow::OnMouseFocus(EventArgs& e)
{
	MouseFocus(e);
}

// The window has lost mouse focus
void RenderWindow::OnMouseBlur(EventArgs& e)
{
	MouseBlur(e);
}

void RenderWindow::OnClose(WindowCloseEventArgs& e)
{
	Close(e);
}
