#include "stdafx.h"

// General
#include "GameState.h"

CGameState::CGameState() : 
	m_Window(nullptr),
	m_IsInited(false), 
	m_IsCurrent(false),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>()),
	m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{
	m_Engine = GetManager<IEngine>();
}

CGameState::~CGameState()
{
	OW_SAFEDELETE(m_Window);
}

bool CGameState::Init()
{
    m_Window = new UIWindow(GetManager<IUIMgr>());
    m_Window->Init(vec2(0.0f, 0.0f), vec2(m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY), nullptr);

    m_IsInited = true;

    return true;
}

void CGameState::Destroy()
{
	delete m_Window;
}

bool CGameState::Set()
{
	_Bindings->RegisterUpdatableObject(this);
	_Bindings->RegisterRenderableUIObject(this, 100);
	_Bindings->RegisterInputListener(this);

    GetManager<IUIMgr>()->SetRootElement(m_Window);

    return true;
}

void CGameState::Unset()
{
	_Bindings->UnregisterUpdatableObject(this);
	_Bindings->UnregisterRenderableUIObject(this);
	_Bindings->UnregisterInputListener(this);
}
