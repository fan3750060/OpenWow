#include "stdafx.h"

// Additional
#include "GameState_Menu.h"
#include "GameState_InWorld.h"
#include <ctime>

/*
	Инициализация:
	1) BaseManager
	2) Хелперы
	3) Ввод/вывод
*/

int main(int argumentCount, char* arguments[])
{
#ifdef _DEBUG 
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
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

		UIMgr uiMgrEx;

		OpenGLAdapter_GLFW adapter;

		CEngine engine;
		engine.Init(&adapter);
		engine.SetArguments(argumentCount, arguments);

		OpenDBs();

		GameStateManager gsManager;
		gsManager.AddGameState(GameStatesNames::GAME_STATE_MENU, new GameState_Menu());
		gsManager.AddGameState(GameStatesNames::GAME_STATE_WORLD, new GameState_InWorld());
		gsManager.SetGameState(GameStatesNames::GAME_STATE_MENU);

		while (engine.Tick());
	}

	return 0;
}
