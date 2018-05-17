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
    UpdatableObject::Register();
    RenderableUIObject::Register(100);
    InputListenerObject::Register();

    _UIMgr->SetRootElement(m_Window);

    return true;
}

void GameState::Unset()
{
    UpdatableObject::Unregister();
    RenderableUIObject::Unregister();
    InputListenerObject::Unregister();
}
