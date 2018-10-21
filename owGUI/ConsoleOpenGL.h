#pragma once

struct ConsoleMessage
{
	ConsoleMessage(cstring _text, const Color _color)
	{
		text = _text;
		color = _color;
	}

	std::string text;
	Color color;
};

//

#include "Font.h"

class CConsoleOpenGL : public IInputListener, public CDebugOutput
{
	typedef vector<ConsoleMessage> ConsoleMessages;
public:

	CConsoleOpenGL();
	virtual ~CConsoleOpenGL();

	void RenderUI();

	// IInputListener
	void OnMouseMoved(cvec2 _mousePos) override {}
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override { return m_IsOpened; }
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override { return m_IsOpened; }
	bool OnMouseWheel(int _yoffset) override;
	bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override { return m_IsOpened; }
	bool OnCharInput(uint32 _char) override;

protected:
	// CDebugOutput
	void Print(std::string _messageFmt, CDebugOutput::DebugMessageType _type);

private:
	vec2 windowSize;
	SharedFontPtr consoleFont;

	bool m_IsOpened;
	
	int fontHeight;
	int linesInConsole;
	int consoleHeight;
	vec2 textOffset;

	int lineOffset;

	ConsoleMessages messages;
	std::string inputString;

	// Help commands
	vector<IConsoleCommand*> commandsHelper;
	vec2 helperOffset;
	int helperSelected;

	IUIMgr*   m_UIMgr;
	IConsole* m_MainConsole;
};
