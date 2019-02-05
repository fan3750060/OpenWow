#include "stdafx.h"

// General
#include "GameState.h"

// Additional
#include "Application.h"

CGameState::CGameState() 
	: m_IsInited(false)
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
	std::shared_ptr<RenderWindow> renderWindow = Application::Get().GetRenderWindow();

	renderWindow->PreRender += boost::bind(&CGameState::OnPreRender, this, _1);
	renderWindow->Render += boost::bind(&CGameState::OnRender, this, _1);
	renderWindow->PostRender += boost::bind(&CGameState::OnPostRender, this, _1);
	renderWindow->RenderUI += boost::bind(&CGameState::OnRenderUI, this, _1);

    return true;
}

void CGameState::Unset()
{
	std::shared_ptr<RenderWindow> renderWindow = Application::Get().GetRenderWindow();
	
	renderWindow->PreRender -= boost::bind(&CGameState::OnPreRender, this, _1);
	renderWindow->Render -= boost::bind(&CGameState::OnRender, this, _1);
	renderWindow->PostRender -= boost::bind(&CGameState::OnPostRender, this, _1);
	renderWindow->RenderUI -= boost::bind(&CGameState::OnRenderUI, this, _1);
}
