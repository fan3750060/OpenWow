#include "stdafx.h"

// Additional (OW)
#include "GameState_Client.h"
#include "GameState_World.h"

// Additional (Windows)
#include <ctime>

int main(int argumentCount, char* arguments[])
{
#ifdef _DEBUG 
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
	{
		_BaseManager = std::make_shared<CBaseManager>();

		std::shared_ptr<CSettings> settings = std::make_shared<CSettings>();
		AddManager<ISettings>(settings);
		settings->AddDefaults();
		

		std::shared_ptr<CLog> log = std::make_shared<CLog>();
		AddManager<ILog>(log);

		std::shared_ptr<CConsole> console = std::make_shared<CConsole>();
		AddManager<IConsole>(console);
		console->AddCommonCommands();
		
		std::shared_ptr<IMPQArchiveManager> mpqArchiveManager = std::make_shared<CMPQArchiveManager>();
		AddManager<IMPQArchiveManager>(mpqArchiveManager);

		std::shared_ptr<IFilesManager> filesManager = std::make_shared<CFilesManager>();
		AddManager<IFilesManager>(filesManager);

		OpenDBs();

		//--

		Application app;
		app.Load();
		app.AddGameState(GameStatesNames::GAME_STATE_WORLD, std::make_shared<CGameState_World>());
		app.AddGameState(GameStatesNames::GAME_STATE_CLIENT, std::make_shared<CGameState_Client>());
		app.SetGameState(GameStatesNames::GAME_STATE_CLIENT);
		app.Run();
	}

	return 0;
}
