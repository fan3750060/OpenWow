#pragma once

class Console;
class IInputListener;
class DebugOutput;

//

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

class ConsoleOpenGL : public Console, public RenderableUIObject, public IInputListener, public DebugOutput
{
	typedef vector<ConsoleMessage*> ConsoleMessages;
public:

	ConsoleOpenGL();
	~ConsoleOpenGL();

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
	void Print(string _messageFmt, DebugOutput::DebugMessageType _type);

private:
	vec2 windowSize;

	bool opened;
	Font* consoleFont;
	int fontHeight;
	int linesInConsole;
	int consoleHeight;
	vec2 textOffset;

	int lineOffset;

	ConsoleMessages messages;
	string inputString;

	// Help commands
	vector<ConsoleCommand*> commandsHelper;
	vec2 helperOffset;
	int helperSelected;
};
