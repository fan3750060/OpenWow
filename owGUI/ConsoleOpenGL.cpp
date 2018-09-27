#include "stdafx.h"

// General
#include "ConsoleOpenGL.h"

CConsoleOpenGL::CConsoleOpenGL() :
	windowSize(GetSettingsGroup<CGroupVideo>().GetWindowSize()),
	m_UIMgr(GetManager<IUIMgr>()),
	m_MainConsole(GetManager<IConsole>())
{
	m_IsOpened = false;
	consoleFont = GetManager<IFontsManager>()->GetMainFont();
	_ASSERT(consoleFont != nullptr);
	fontHeight = consoleFont->GetHeight();
	consoleHeight = windowSize.y / 2.0f;
	linesInConsole = consoleHeight / fontHeight;
	textOffset = vec2(5.0f, -(fontHeight + 3.0f));
	lineOffset = 0;
	inputString = "";

	helperSelected = 0;
	helperOffset = vec2(consoleFont->GetStringWidth(">"), 0.0f);

    //

	GetManager<ILog>()->AddDebugOutput(this);
	_Bindings->RegisterInputListener(this);
}

CConsoleOpenGL::~CConsoleOpenGL()
{
	GetManager<ILog>()->DeleteDebugOutput(this);

	_Bindings->UnregisterInputListener(this);
}

void CConsoleOpenGL::RenderUI()
{
	if (!m_IsOpened)
	{
		return;
	}

	// Console rectangle
	_Render->RenderRectangle(vec2(), vec2(windowSize.x, consoleHeight), COLOR_BLACK);

	// Messages
	int curLine = 0;
	for (uint32_t i = static_cast<uint32>(messages.size()) - 1 - lineOffset; i > 0; i--)
	{
		auto _message = messages[i];
		if (curLine == linesInConsole)
		{
			break;
		}
		m_UIMgr->RenderText(textOffset + vec2(0, consoleHeight - (curLine * fontHeight)), _message.text, consoleFont, _message.color);
		curLine++;
	}

	// Input rectangle
	_Render->RenderRectangle(vec2(0.0f, consoleHeight), vec2(windowSize.x, fontHeight), COLOR_DARKGRAY);

	// Input string
	m_UIMgr->RenderText(vec2(0.0f, consoleHeight), ">" + inputString, COLOR_WHITE);

	// Helper
	if (!commandsHelper.empty())
	{
		unsigned i = 1;
		for (auto it = commandsHelper.begin(); it != commandsHelper.end(); ++it)
		{
			auto command = *it;
			auto commandName = command->GetName();
			auto commandNameWidth = consoleFont->GetStringWidth(commandName);

			// Rectangle
			_Render->RenderRectangle(vec2(0.0f, consoleHeight + i * fontHeight), helperOffset + vec2(commandNameWidth, fontHeight), COLOR_GRAY);

			// Selected
			if (helperSelected == i)
			{
				m_UIMgr->RenderText(vec2(0.0f, consoleHeight + i * fontHeight), ">", COLOR_WHITE);
				_Render->RenderRectangleOutline(vec2(0.0f, consoleHeight + i * fontHeight), helperOffset + vec2(commandNameWidth, fontHeight), COLOR_BLUE);
			}

			// Text
			string line = commandName;
			if (command->HasArgs())
			{
				line += " [args]";
			}
			m_UIMgr->RenderText(helperOffset + vec2(0.0f, consoleHeight + i * fontHeight), line, COLOR_WHITE);

			i++;
		}
	}
}

//

bool CConsoleOpenGL::OnMouseWheel(int _yoffset)
{
	if (!m_IsOpened)
	{
		return false;
	}

	if (messages.size() < linesInConsole)
	{
		return true;
	}

	lineOffset += _yoffset;
	lineOffset = clamp(lineOffset, 0, (int)messages.size() - (int)linesInConsole - 1);

	return true;
}

bool CConsoleOpenGL::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
	if (_key == OW_KEY_GRAVE_ACCENT)
	{
		m_IsOpened = !m_IsOpened;
		inputString = "";
		commandsHelper.clear();
		helperSelected = 0;
		return true;
	}

	if (_key == OW_KEY_BACKSPACE && !inputString.empty())
	{
		inputString.pop_back();

		if (!inputString.empty())
		{
			if (inputString.back() != ' ')
				commandsHelper = m_MainConsole->GetConsoleCommandHelp(inputString);
			else
				commandsHelper.clear();
			helperSelected = 0;
		}

		return true;
	}

	if (_key == OW_KEY_DOWN)
	{
		helperSelected++;
		helperSelected = minf(helperSelected, (int)commandsHelper.size());
	}

	if (_key == OW_KEY_UP)
	{
		helperSelected--;
		helperSelected = maxf(helperSelected, 0);
	}

	if (_key == OW_KEY_ENTER && !inputString.empty())
	{
		int helperSelectedIndex = helperSelected - 1;
		if (helperSelectedIndex != -1 && helperSelectedIndex < commandsHelper.size())
		{
			inputString = commandsHelper[helperSelectedIndex]->GetName() + " ";
			commandsHelper.clear();
			helperSelected = 0;
			return true;
		}

		Log::Print(inputString.c_str());
		m_MainConsole->ProcessConsoleCommand(inputString);
		inputString = "";
		return true;
	}

	return m_IsOpened;
}

bool CConsoleOpenGL::OnCharInput(uint32 _char)
{
    if (!m_IsOpened)
    {
        return false;
    }

    if (_char == '`')
    {
        return false;
    }

	// If empty line then skip space
    if (inputString.empty() && _char == ' ')
    {
        return true;
    }

	// Skip multiple spaces
	if (!inputString.empty())
	{
		if (inputString.back() == ' ' && _char == ' ')
			return true;
	}

	inputString += _char;

	if (inputString.back() != ' ')
		commandsHelper = m_MainConsole->GetConsoleCommandHelp(inputString);
	else
		commandsHelper.clear();
	helperSelected = 0;

	return true;
}

//

void CConsoleOpenGL::Print(string _messageFmt, DebugMessageType _type)
{
	Color color = COLOR_WHITE;

	switch (_type)
	{
		case TYPE_INFO:
		color = COLOR_GRAY;
		break;

		case TYPE_PRINT:
		color = COLOR_WHITE;
		break;

		case TYPE_GREEN:
		color = COLOR_GREEN;
		break;

		case TYPE_WARNING:
		color = COLOR_YELLOW;
		break;

		case TYPE_ERROR:
		case TYPE_FATAL:
		color = COLOR_RED;
		break;
	}

	messages.push_back(ConsoleMessage(_messageFmt, color));
}