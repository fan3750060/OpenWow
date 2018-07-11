#include "stdafx.h"

// General
#include "GameStateManager.h"

GameStateManager::GameStateManager() :
	m_CurrentGameState(nullptr)
{
	AddManager<IGameStateManager>(this);
}

GameStateManager::~GameStateManager()
{
	ERASE_MAP(m_GameStatesCollection);
}

void GameStateManager::AddGameState(GameStatesNames::List _name, IGameState* _gameState)
{
    assert1(_gameState != nullptr);
    assert1(m_GameStatesCollection.find(_name) == m_GameStatesCollection.end());

    m_GameStatesCollection.insert(make_pair(_name, _gameState));
}

bool GameStateManager::SetGameState(GameStatesNames::List _name)
{
    assert1(m_GameStatesCollection.find(_name) != m_GameStatesCollection.end());

    IGameState* gameState = m_GameStatesCollection[_name];
    return SetGameState(gameState);
}

bool GameStateManager::SetGameState(IGameState* _newGameState)
{
    assert1(_newGameState);

    Log::Print("GameStateManager[]: Setting new CGameState.");

    // 1. Unset current GameState
    if (m_CurrentGameState != nullptr)
    {
        m_CurrentGameState->Unset();
        m_CurrentGameState->SetCurrent(false);
    }

    // 2. If new GameState not inited, init them
    if (!_newGameState->IsInited())
    {
        Log::Warn("GameStateManager[]: New CGameState in not inited. Initializating.");
        if (_newGameState->Init())
        {
			Log::Green("GameStateManager[]: New CGameState is inited successfully.");
			_newGameState->SetInited(true);
        }
        else
        {
            Log::Error("GameStateManager[]: Error initing new CGameState.");
        }
    }

    // 3. Set new GameState
    m_CurrentGameState = dynamic_cast<CGameState*>(_newGameState);
    if (m_CurrentGameState->Set())
    {
		Log::Green("GameStateManager[]: New CGameState is current now.");
        m_CurrentGameState->SetCurrent(true);
    }
    else
    {
        Log::Error("CEngine[]: Error setting current new CGameState.");
    }

    return true;
}