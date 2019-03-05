#include "stdafx.h"

// General
#include "GameState.h"

// Additional
#include "Application.h"

CGameState::CGameState(const IApplication * _application)
	: m_Application(_application)
	, m_IsInited(false)
	, m_IsCurrent(false)
	, m_QualitySettings(GetSettingsGroup<CGroupQuality>())
	, m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{}

CGameState::~CGameState()
{}



//
// IGameState
//
bool CGameState::Init()
{
    m_IsInited = true;

    return true;
}

void CGameState::Destroy()
{

}

bool CGameState::Set()
{
	std::shared_ptr<RenderWindow> renderWindow = m_Application->GetRenderWindow();

	// Input events connections
	OnMouseButtonPressedConnection  = renderWindow->MouseButtonPressed += boost::bind(&CGameState::OnMouseButtonPressed, this, _1);
	OnMouseButtonReleasedConnection = renderWindow->MouseButtonReleased += boost::bind(&CGameState::OnMouseButtonReleased, this, _1);
	OnMouseMovedConnection          = renderWindow->MouseMoved += boost::bind(&CGameState::OnMouseMoved, this, _1);
	OnMouseWheelConnection          = renderWindow->MouseWheel += boost::bind(&CGameState::OnMouseWheel, this, _1);
	OnKeyPressedConnection          = renderWindow->KeyPressed += boost::bind(&CGameState::OnKeyPressed, this, _1);
	OnKeyReleasedConnection         = renderWindow->KeyReleased += boost::bind(&CGameState::OnKeyReleased, this, _1);

	// Window events connections
	OnResizeConnection              = renderWindow->Resize += boost::bind(&CGameState::OnResize, this, _1);

	// Update events connection
	OnUpdateConnection              = renderWindow->Update += boost::bind(&CGameState::OnUpdate, this, _1);

	// Render events connections
	OnPreRenderConnection           = renderWindow->PreRender += boost::bind(&CGameState::OnPreRender, this, _1);
	OnRenderConnection              = renderWindow->Render += boost::bind(&CGameState::OnRender, this, _1);
	OnPostRenderConnection          = renderWindow->PostRender += boost::bind(&CGameState::OnPostRender, this, _1);
	OnRenderUIConnection            = renderWindow->RenderUI += boost::bind(&CGameState::OnRenderUI, this, _1);

    return true;
}

void CGameState::Unset()
{
	/*_renderWindow->Update -= OnUpdateConnection;
	_renderWindow->MouseButtonPressed -= OnMouseButtonPressedConnection;
	_renderWindow->MouseButtonReleased -= OnMouseButtonReleasedConnection;
	_renderWindow->MouseMoved -= OnMouseMovedConnection;
	_renderWindow->MouseWheel -= OnMouseWheelConnection;
	_renderWindow->KeyPressed -= OnKeyPressedConnection;
	_renderWindow->KeyReleased -= OnKeyReleasedConnection;*/

}



//
// Input events
//
void CGameState::OnKeyPressed(KeyEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnKeyPressed(e);
}

void CGameState::OnKeyReleased(KeyEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnKeyReleased(e);
}

void CGameState::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseButtonPressed(e);
}

void CGameState::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseButtonReleased(e);
}

void CGameState::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseMoved(e);
}

void CGameState::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseWheel(e);
}



//
// Window events
//
void CGameState::OnResize(ResizeEventArgs & e)
{
	m_Viewport.Width = e.Width;
	m_Viewport.Height = e.Height;
	m_Viewport.OrthoMatrix = glm::ortho(0.0f, m_Viewport.Width, m_Viewport.Height, 0.0f, -1.0f, 1.0f);
}



//
// Update event
//
void CGameState::OnUpdate(UpdateEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnUpdate(e);
}




void CGameState::SetCameraController(std::shared_ptr<ICameraController> CameraController)
{
	_ASSERT(CameraController != nullptr);
	m_DefaultCameraController = CameraController;
}

void CGameState::UnsetCameraController()
{
	m_DefaultCameraController = nullptr;
}
