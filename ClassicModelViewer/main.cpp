#include "stdafx.h"

// Additional
#include "GameState_M2Viewer.h"
#include <ctime>

int main(int argumentCount, char* arguments[])
{
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	CSettings settings;
	settings.AddDefaults();

	CWindowsPlatformFactory winPlatformFactory;

	CLog log;
	log.AddDebugOutput(winPlatformFactory.CreateDebugOutputConsole());

	CConsole console;
	console.AddCommonCommands();

	CMPQArchiveManager mpqArchiveManager;
	
	CFilesManager filesManager;

	CEngine engine(winPlatformFactory.CreateOpenGLAdapter());
	engine.SetArguments(argumentCount, arguments);

	CConsoleOpenGL openGLConsole;
	log.AddDebugOutput(&openGLConsole);

	GameStateManager gsManager;
	gsManager.AddGameState(GameStatesNames::GAME_STATE_MENU, new GameState_M2Viewer());
	gsManager.SetGameState(GameStatesNames::GAME_STATE_MENU);

	while (engine.Tick());

	return 0;
}
