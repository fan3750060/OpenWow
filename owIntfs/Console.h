#pragma once

#include "ConsoleCommand.h"

// Forward BEGIN
__interface IManager;
// Forward END

typedef vector<IConsoleCommand*> ConsoleCommands;

__interface 
	__declspec(uuid("1CB48B2B-357E-4B11-8587-3D6A8385A436"))
	IConsole : public IManager
{
	bool AddConsoleCommand(IConsoleCommand* _command);
	IConsoleCommand* GetConsoleCommandByName(cstring _commandName);
	ConsoleCommands GetConsoleCommandHelp(string _input);
	bool ProcessConsoleCommand(string _line);
};