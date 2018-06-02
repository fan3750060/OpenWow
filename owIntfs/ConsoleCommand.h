#pragma once

__interface
	__declspec(uuid("52B0DE8F-A0C3-441F-BA1F-6307A9AF0096"))
	IConsoleCommand
{
	const string GetName() const;
	bool HasArgs() const;
	void Execute();
	void Execute(cstring _args);
};