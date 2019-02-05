#include "stdafx.h"

// General
#include "GameStateManager.h"

CGameStateManager::CGameStateManager() :
	m_CurrentGameState(nullptr)
{

}

CGameStateManager::~CGameStateManager()
{
	DelManager<IGameStateManager>();
}

void CGameStateManager::AddGameState(GameStatesNames::List _name, std::shared_ptr<IGameState> _gameState)
{
    assert1(_gameState != nullptr);
    assert1(m_GameStatesCollection.find(_name) == m_GameStatesCollection.end());

    m_GameStatesCollection.insert(std::make_pair(_name, _gameState));
}

bool CGameStateManager::SetGameState(GameStatesNames::List _name)
{
    assert1(m_GameStatesCollection.find(_name) != m_GameStatesCollection.end());

	std::shared_ptr<IGameState> gameState = m_GameStatesCollection[_name];
    return SetGameState(gameState);
}

bool CGameStateManager::SetGameState(std::shared_ptr<IGameState> _newGameState)
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
    m_CurrentGameState = std::dynamic_pointer_cast<CGameState, IGameState>(_newGameState);
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