#pragma once

struct ConsoleMessage
{
	ConsoleMessage(cstring _text, const Color _color)
	{
		text = _text;
		color = _color;
	}

	string text;
	Color color;
};

//

class CConsoleOpenGL : public CRenderableUIObject, public IInputListener, public CDebugOutput
{
	typedef vector<ConsoleMessage*> ConsoleMessages;
public:

	CConsoleOpenGL();
	~CConsoleOpenGL();

	// IRenderableUI
	void RenderUI() override;

	// IInputListener
	void OnMouseMoved(cvec2 _mousePos) override {}
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override { return false; }
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override { return false; }
	bool OnMouseWheel(int _yoffset) override;
	bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override { return false; }
	bool OnCharInput(uint32 _char) override;

protected:
	// CDebugOutput
	void Print(string _messageFmt, CDebugOutput::DebugMessageType _type);

private:
	vec2 windowSize;
	Font* consoleFont;

	bool m_IsOpened;
	
	int fontHeight;
	int linesInConsole;
	int consoleHeight;
	vec2 textOffset;

	int lineOffset;

	ConsoleMessages messages;
	string inputString;

	// Help commands
	vector<IConsoleCommand*> commandsHelper;
	vec2 helperOffset;
	int helperSelected;

	IConsole* m_MainConsole;
};
