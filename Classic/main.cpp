#include "stdafx.h"

// Additional
#include "GameState_Menu.h"
#include "GameState_InWorld.h"
#include <ctime>

int main(int argumentCount, char* arguments[])
{
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	CSettings settings;
	AddSettingsGroup<CGroupDistances>(new CGroupDistances);
	AddSettingsGroup<CGroupOpenGL>(new CGroupOpenGL);
	AddSettingsGroup<CGroupQuality>(new CGroupQuality);
	AddSettingsGroup<CGroupRenderCaps>(new CGroupRenderCaps);
	AddSettingsGroup<CGroupVideo>(new CGroupVideo);

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

    //----------------------------------------------------------------
	GameStateManager gsManager;
	gsManager.AddGameState(GameStatesNames::GAME_STATE_MENU, new GameState_Menu());
	gsManager.AddGameState(GameStatesNames::GAME_STATE_WORLD, new GameState_InWorld());
	gsManager.SetGameState(GameStatesNames::GAME_STATE_MENU);

	while (engine.Tick());

	return 0;
}
