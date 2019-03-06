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
	renderWindow->MouseButtonPressed += std::bind(&CGameState::OnMouseButtonPressed, this, std::placeholders::_1);
	renderWindow->MouseButtonReleased += std::bind(&CGameState::OnMouseButtonReleased, this, std::placeholders::_1);
	renderWindow->MouseMoved += std::bind(&CGameState::OnMouseMoved, this, std::placeholders::_1);
	renderWindow->MouseWheel += std::bind(&CGameState::OnMouseWheel, this, std::placeholders::_1);
	renderWindow->KeyPressed += std::bind(&CGameState::OnKeyPressed, this, std::placeholders::_1);
	renderWindow->KeyReleased += std::bind(&CGameState::OnKeyReleased, this, std::placeholders::_1);

	// Window events connections
	renderWindow->Resize += std::bind(&CGameState::OnResize, this, std::placeholders::_1);

	// Update events connection
	renderWindow->Update += std::bind(&CGameState::OnUpdate, this, std::placeholders::_1);

	// Render events connections
	renderWindow->PreRender += std::bind(&CGameState::OnPreRender, this, std::placeholders::_1);
	renderWindow->Render += std::bind(&CGameState::OnRender, this, std::placeholders::_1);
	renderWindow->PostRender += std::bind(&CGameState::OnPostRender, this, std::placeholders::_1);
	renderWindow->RenderUI += std::bind(&CGameState::OnRenderUI, this, std::placeholders::_1);

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
