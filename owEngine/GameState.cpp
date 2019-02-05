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
{

}

CGameState::~CGameState()
{

}

bool CGameState::Init()
{
	Application::Get().GetRenderWindow()->PreRender += boost::bind(&CGameState::OnPreRender, this, _1);
	Application::Get().GetRenderWindow()->Render += boost::bind(&CGameState::OnRender, this, _1);
	Application::Get().GetRenderWindow()->PostRender += boost::bind(&CGameState::OnPostRender, this, _1);
	Application::Get().GetRenderWindow()->RenderUI += boost::bind(&CGameState::OnRenderUI, this, _1);

    m_IsInited = true;

    return true;
}

void CGameState::Destroy()
{

}

bool CGameState::Set()
{
    return true;
}

void CGameState::Unset()
{

}
