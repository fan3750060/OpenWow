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
	m_Application->GetRenderWindow()->Resize += boost::bind(&CGameState::OnResize, this, _1);

	m_Application->GetRenderWindow()->PreRender += boost::bind(&CGameState::OnPreRender, this, _1);
	m_Application->GetRenderWindow()->Render += boost::bind(&CGameState::OnRender, this, _1);
	m_Application->GetRenderWindow()->PostRender += boost::bind(&CGameState::OnPostRender, this, _1);
	m_Application->GetRenderWindow()->RenderUI += boost::bind(&CGameState::OnRenderUI, this, _1);

    return true;
}

void CGameState::Unset()
{

	//renderWindow->PreRender -= boost::bind(&CGameState::OnPreRender, this, _1);
	//renderWindow->Render -= boost::bind(&CGameState::OnRender, this, _1);
	//renderWindow->PostRender -= boost::bind(&CGameState::OnPostRender, this, _1);
	//renderWindow->RenderUI -= boost::bind(&CGameState::OnRenderUI, this, _1);
}

void CGameState::OnResize(ResizeEventArgs & e)
{
	m_Viewport.Width = e.Width;
	m_Viewport.Height = e.Height;
	m_Viewport.OrthoMatrix = glm::ortho(0.0f, m_Viewport.Width, m_Viewport.Height, 0.0f, -1.0f, 1.0f);
}
