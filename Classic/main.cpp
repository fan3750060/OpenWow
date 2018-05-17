#include "stdafx.h"

// Additional
#include "GameState_Menu.h"
#include "GameState_InWorld.h"

int main(int argumentCount, char* arguments[])
{
	Log::AddDebugOutput(PlatformWindows::CreateDebugOutput_ConsoleWindows());
	//Log::AddDebugOutput(PlatformWindows::CreateDebugOutput_Log());

	Engine* engine = new Engine(PlatformWindows::CreateAdapter_GLFW(), argumentCount, arguments);
    
    //----------------------------------------------------------------

    _UIMgr->Init();

    //----------------------------------------------------------------

    // Custom debug outputs
	ConsoleOpenGL openGLConsole;
	Log::AddDebugOutput(&openGLConsole);

    //----------------------------------------------------------------

    GameStateManager::AddGameState(GameStatesNames::GAME_STATE_MENU, new GameState_Menu(engine));
	GameStateManager::AddGameState(GameStatesNames::GAME_STATE_WORLD, new GameState_InWorld(engine));
    GameStateManager::SetGameState(GameStatesNames::GAME_STATE_MENU);

    //----------------------------------------------------------------

	while (engine->Tick());

    //----------------------------------------------------------------

	delete engine;
}
