#pragma once

#include "ConsoleCommand.h"

// Forward BEGIN
__interface IManager;
// Forward END

typedef vector<IConsoleCommand*> ConsoleCommands;

struct 
	__declspec(uuid("1CB48B2B-357E-4B11-8587-3D6A8385A436"))
	IConsole : public IManager
{
	virtual ~IConsole() {}

	virtual bool AddConsoleCommand(IConsoleCommand* _command) = 0;
	virtual IConsoleCommand* GetConsoleCommandByName(cstring _commandName) = 0;
	virtual ConsoleCommands GetConsoleCommandHelp(string _input) = 0;
	virtual bool ProcessConsoleCommand(string _line) = 0;
};