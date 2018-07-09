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