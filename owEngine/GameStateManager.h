#pragma once

#include "GameState.h"

class CGameStateManager : public IGameStateManager
{
public:
	CGameStateManager();
	virtual ~CGameStateManager();

	// IGameStateManager
    void AddGameState(GameStatesNames::List _name, std::shared_ptr<IGameState> _gameState) override;
    bool SetGameState(GameStatesNames::List _name) override;
	bool SetGameState(std::shared_ptr<IGameState> _newGameState) override;
    std::shared_ptr<IGameState> GetGameState() override { return m_CurrentGameState; }

private:
	std::shared_ptr<IGameState>                                     m_CurrentGameState;
	std::map<GameStatesNames::List, std::shared_ptr<IGameState>>    m_GameStatesCollection;
};