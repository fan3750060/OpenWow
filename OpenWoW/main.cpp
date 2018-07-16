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
		Random::SetSeed(static_cast<unsigned long>(time(0)));

		CBaseManager baseManager;
		_BaseManager = &baseManager;

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

		GameState_Menu gsMenu;
		GameState_InWorld gsWorld;

		GameStateManager gsManager;
		gsManager.AddGameState(GameStatesNames::GAME_STATE_MENU, &gsMenu);
		gsManager.AddGameState(GameStatesNames::GAME_STATE_WORLD, &gsWorld);
		gsManager.SetGameState(GameStatesNames::GAME_STATE_MENU);

		while (engine.Tick());
	}

	return 0;
}
