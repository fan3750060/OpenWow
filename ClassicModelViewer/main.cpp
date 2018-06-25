#include "stdafx.h"

// Additional
#include "GameState_M2Viewer.h"
#include "GameState_WMOViewer.h"
#include <ctime>

int main(int argumentCount, char* arguments[])
{
	Random::SetSeed(static_cast<unsigned long>(time(0)));

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

	CEngine engine;
	engine.Init(new OpenGLAdapter_GLFW());
	engine.SetArguments(argumentCount, arguments);

	CConsoleOpenGL openGLConsole;
	log.AddDebugOutput(&openGLConsole);

	OpenDBs();

	GameStateManager gsManager;
	gsManager.AddGameState(GameStatesNames::GAME_STATE_MENU, new GameState_M2Viewer());
	gsManager.AddGameState(GameStatesNames::GAME_STATE_WORLD, new GameState_WMOViewer());
	gsManager.SetGameState(GameStatesNames::GAME_STATE_MENU);

	while (engine.Tick());

	return 0;
}
