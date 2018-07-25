#include "stdafx.h"

// Additional
#include "GameState_WMOViewer.h"
#include "GameState_M2Viewer.h"
#include "GameState_CharacterViewer.h"
#include "GameState_CreaturesViewer.h"

#include "GameState_CreateCharacter.h"

#include <ctime>

int main(int argumentCount, char* arguments[])
{
#ifdef _DEBUG 
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
	{
		CBaseManager baseManager;
		_BaseManager = &baseManager;

		Random::SetSeed(static_cast<unsigned long>(time(0)));

		CBindingController bindingController;
		_Bindings = &bindingController;

		CSettings settings;
		settings.AddDefaults();

		CLog log;

		CConsole console;
		console.AddCommonCommands();

		HANDLE hProcess = GetCurrentProcess();
		if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
		{
			Log::Info("Process priority class set to HIGH");
		}
		else
		{
			Log::Error("Can't set process priority class.");
		}

		CMPQArchiveManager mpqArchiveManager;

		CFilesManager filesManager;

		CEngine engine;
		engine.Init(new OpenGLAdapter_GLFW());
		engine.SetArguments(argumentCount, arguments);

		OpenDBs();

		GameState_WMOViewer gsWMOWiew;
		GameState_M2Viewer gsM2Wiew;
		GameState_CharacterViewer gsCharacterWiew;
		GameState_CreaturesViewer gsCreaturesWiew;
		GameState_CreateCharacter gsCreateCharacter;

		GameStateManager gsManager;
		gsManager.AddGameState(GameStatesNames::GAME_STATE_WMO_VIEW, &gsWMOWiew);
		gsManager.AddGameState(GameStatesNames::GAME_STATE_M2_VIEW, &gsM2Wiew);
		gsManager.AddGameState(GameStatesNames::GAME_STATE_CHARACTER_VIEW, &gsCharacterWiew);
		gsManager.AddGameState(GameStatesNames::GAME_STATE_CREATURE_VIEW, &gsCreaturesWiew);
		gsManager.AddGameState(GameStatesNames::GAME_STATE_CREATE_CHARACTER, &gsCreateCharacter);

		gsManager.SetGameState(GameStatesNames::GAME_STATE_M2_VIEW);

		while (engine.Tick());
	}

	return 0;
}
