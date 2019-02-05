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
{}

RenderWindow::~RenderWindow()
{}

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
	::GetClientRect(m_hWindow, &rc); // get client coords
	::ClientToScreen(m_hWindow, reinterpret_cast<POINT*>(&rc.left)); // convert top-left
	::ClientToScreen(m_hWindow, reinterpret_cast<POINT*>(&rc.right)); // convert bottom-right

	::SetCursorPos(rc.left + _position.x, rc.top + _position.y);
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

	::SetWindowText(m_hWindow, m_sWindowName.c_str());
}

cstring RenderWindow::GetWindowName() const
{
	return m_sWindowName;
}



//
// Engine events
//
void RenderWindow::OnInitialize(EventArgs& e)
{
	Initialize(e);
}

void RenderWindow::OnUpdate(UpdateEventArgs& e)
{
	Update(e);
}

void RenderWindow::OnPreRender(Render3DEventArgs& e)
{
	Render3DEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState);
	PreRender(renderArgs);
}

void RenderWindow::OnRender(Render3DEventArgs& e)
{
	Render3DEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState);
	Render(renderArgs);
}

void RenderWindow::OnPostRender(Render3DEventArgs& e)
{
	Render3DEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState);
	PostRender(renderArgs);
}

void RenderWindow::OnRenderUI(RenderUIEventArgs& e)
{
	RenderUIEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter);
	RenderUI(renderArgs);
}

void RenderWindow::OnTerminate(EventArgs& e)
{
	Terminate(e);
}



//
// Window events
//
void RenderWindow::OnInputFocus(EventArgs& e) // The window has received focus
{
	InputFocus(e);
}

void RenderWindow::OnInputBlur(EventArgs& e) // The RenderWindow window has lost focus
{
	InputBlur(e);
}

void RenderWindow::OnMinimize(EventArgs& e) // The RenderWindow window has been minimized
{
	Minimize(e);
}

void RenderWindow::OnRestore(EventArgs& e) // The RenderWindow window has been restored
{
	Restore(e);
}

void RenderWindow::OnResize(ResizeEventArgs& e) // The RenderWindow window has be resized
{
	m_iWindowWidth = e.Width;
	m_iWindowHeight = e.Height;

	Resize(e);
}

void RenderWindow::OnExpose(EventArgs& e) // The window contents should be repainted
{
	Expose(e);
}

void RenderWindow::OnClose(WindowCloseEventArgs& e)
{
	Close(e);
}



//
// Keyboard events
//
void RenderWindow::OnKeyPressed(KeyEventArgs& e) // A keyboard key was pressed
{
	KeyPressed(e);
}

void RenderWindow::OnKeyReleased(KeyEventArgs& e) // A keyboard key was released
{
	KeyReleased(e);
}

void RenderWindow::OnKeyboardFocus(EventArgs& e) // Window gained keyboard focus
{
	m_bHasKeyboardFocus = true;
	KeyboardFocus(e);
}

void RenderWindow::OnKeyboardBlur(EventArgs& e) // Window lost keyboard focus
{
	m_bHasKeyboardFocus = false;
	KeyboardBlur(e);
}


//
// The mouse events
//

void RenderWindow::OnMouseMoved(MouseMotionEventArgs& e) // The mouse was moved
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


void RenderWindow::OnMouseButtonPressed(MouseButtonEventArgs& e) // A button on the mouse was pressed
{
	MouseButtonPressed(e);
}

void RenderWindow::OnMouseButtonReleased(MouseButtonEventArgs& e) // A button on the mouse was released
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

void RenderWindow::OnMouseFocus(EventArgs& e) // The window has received mouse focus
{
	MouseFocus(e);
}

void RenderWindow::OnMouseBlur(EventArgs& e) // The window has lost mouse focus
{
	MouseBlur(e);
}

