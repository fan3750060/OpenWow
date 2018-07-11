#pragma once

struct
	__declspec(uuid("52B0DE8F-A0C3-441F-BA1F-6307A9AF0096"))
	IConsoleCommand
{
	virtual ~IConsoleCommand() {};

	virtual const string GetName() const = 0;
	virtual bool HasArgs() const = 0;
	virtual void Execute() = 0;
	virtual void Execute(cstring _args) = 0;
};

// Forward BEGIN
__interface IManager;
// Forward END

typedef vector<IConsoleCommand*> ConsoleCommands;

struct 
	__declspec(uuid("1CB48B2B-357E-4B11-8587-3D6A8385A436"))
	IConsole : public IManager
{
	virtual ~IConsole() = 0 {};

	virtual bool AddConsoleCommand(IConsoleCommand* _command) = 0;
	virtual IConsoleCommand* GetConsoleCommandByName(cstring _commandName) = 0;
	virtual ConsoleCommands GetConsoleCommandHelp(string _input) = 0;
	virtual bool ProcessConsoleCommand(string _line) = 0;
};