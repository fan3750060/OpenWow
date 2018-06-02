#pragma once

#include "GameState.h"

class GameStateManager : public IGameStateManager
{
public:
	GameStateManager();

	// IGameStateManager
    void AddGameState(GameStatesNames::List _name, IGameState* _gameState) override;
    bool SetGameState(GameStatesNames::List _name) override;
	bool SetGameState(IGameState* _newGameState) override;
    IGameState* GetGameState() override { return m_CurrentGameState; }

private:
    IGameState*									m_CurrentGameState;
    map<GameStatesNames::List, IGameState*>		m_GameStatesCollection;
};