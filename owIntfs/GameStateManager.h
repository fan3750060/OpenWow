#pragma once

#include "GameState.h"

struct GameStatesNames
{
	enum List
	{
		GAME_STATE_MENU,
		GAME_STATE_WORLD
	};
};

// Forward BEGIN
__interface IManager;
// Forward END

__interface 
	__declspec(uuid("03594F17-5474-47F0-9B6A-7842ACDF888A"))
	IGameStateManager : IManager
{
	void AddGameState(GameStatesNames::List _name, IGameState* _gameState);

	bool SetGameState(GameStatesNames::List _name);
	bool SetGameState(IGameState* _newGameState);

	IGameState* GetGameState();
};