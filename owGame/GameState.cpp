#include "stdafx.h"

// General
#include "GameState.h"

GameState::GameState(Engine* _Engine) 
	: m_IsInited(false), 
	m_IsCurrent(false),
	m_Engine(_Engine)
{}

bool GameState::Init()
{
    m_Window = new UIWindow();
    m_Window->Init(vec2(0.0f, 0.0f), vec2(_Config.windowSizeX, _Config.windowSizeY), nullptr);

    m_IsInited = true;

    return true;
}

void GameState::Destroy()
{
}

bool GameState::Set()
{
	_Bindings->RegisterUpdatableObject(this);
	_Bindings->RegisterRenderableUIObject(this, 100);
	_Bindings->RegisterInputListener(this);

    _UIMgr->SetRootElement(m_Window);

    return true;
}

void GameState::Unset()
{
	_Bindings->UnregisterUpdatableObject(this);
	_Bindings->UnregisterRenderableUIObject(this);
	_Bindings->UnregisterInputListener(this);
}
