#include "stdafx.h"

// Additional
#include "GameState_Menu.h"
#include "GameState_InWorld.h"

class Test : public RefItem
{
public:
	Test() { printf("Create\n"); }
	~Test() { printf("Delete\n"); }
	void Act() { printf("Act\n"); }
};

map<int, int> t;

map<int, int>::iterator test()
{
	return t.find(1);
}

int main(int argumentCount, char* arguments[])
{
	{
		map<int, int> tt;
		map<int, int>::iterator y = t.find(2);
		t.find(3);

		SmartItemPtr<Test> tex = new Test();
		tex->Act();

		SmartItemPtr<Test> tex2 = tex;
	}


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
