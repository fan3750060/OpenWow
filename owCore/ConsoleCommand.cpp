#include "stdafx.h"

// General
#include "ConsoleCommand.h"

ConsoleCommand::ConsoleCommand(cstring _commandName, Function* _function, bool _hasArgs) :
	m_Name(Utils::ToLower(_commandName)),
	m_Function(_function),
	m_HasArgs(_hasArgs)
{}

ConsoleCommand::~ConsoleCommand()
{
	delete m_Function;
}

const string ConsoleCommand::GetName() const 
{ 
	return m_Name; 
}

bool ConsoleCommand::HasArgs() const 
{ 
	return m_HasArgs; 
}

void ConsoleCommand::Execute()
{
	m_Function->operator()();
}

void ConsoleCommand::Execute(cstring _args)
{
	fail2("Unable to call ConsoleCommand::Execute(cstring _args) in base class!");
}
