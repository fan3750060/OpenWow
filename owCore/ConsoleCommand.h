#pragma once

class ConsoleCommand : public IConsoleCommand
{
public:
	ConsoleCommand(cstring _commandName, Function* _function, bool _hasArgs = false);
	virtual ~ConsoleCommand();

	// IConsoleCommand
	const std::string GetName() const override;
	bool HasArgs() const override;
	void Execute() override;
	void Execute(cstring _args) override;

protected:
	std::string     m_Name;
	bool       m_HasArgs;
	Function*  m_Function;
};

template <class ARGTYPE>
class ConsoleCommand_WA : public ConsoleCommand
{
public:
	ConsoleCommand_WA(cstring _commandName, Function* _function) :
		ConsoleCommand(_commandName, _function, true)
	{}
	virtual ~ConsoleCommand_WA()
	{}

	// IConsoleCommand
	const std::string GetName() const override { return ConsoleCommand::GetName(); };
	bool HasArgs() const override { return ConsoleCommand::HasArgs(); };
	void Execute() override { return ConsoleCommand::Execute(); };
	void Execute(cstring _args) override
	{
		ARGTYPE value;

		if (Utils::TryParse(typeid(ARGTYPE), _args, &value))
		{
			Function_WA<ARGTYPE>* funcWA = dynamic_cast<Function_WA<ARGTYPE>*>(m_Function);
			assert1(funcWA != nullptr);

			funcWA->operator()(ARGTYPE(value));
		}
		else
		{
			Log::Error("ConsoleCommand_WA[%s]: Can't parse argument [%s] to [%s].", m_Name.c_str(), _args.c_str(), typeid(ARGTYPE).name());
		}
	}
};