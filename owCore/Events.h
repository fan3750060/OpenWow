#pragma once

#include <functional>

#include "KeyCodes.h"

class Object;

template< class ArgumentType >
class Delegate
{
public:
	typedef std::function<void(ArgumentType&)> FunctionType;
	typedef std::shared_ptr<FunctionType> FunctionDecl;
	typedef std::set<FunctionDecl> FunctionsSet;

	FunctionDecl operator += (typename const FunctionType& function)
	{
		FunctionDecl ret = std::make_shared<FunctionType>(function);
		m_Functions.insert(ret);
		return ret;
	}

	void operator -= (typename const FunctionDecl& function)
	{
		FunctionsSet::const_iterator cit = m_Functions.find(_decl);
		if (cit != m_Functions.end())
		{
			m_Functions.erase(cit);
		}
	}

	void operator()(typename ArgumentType& argument)
	{
		std::for_each(
			m_Functions.begin(),
			m_Functions.end(),
			[&argument](const FunctionDecl& _decl) 
			{ 
				(*_decl)(argument); 
			}
		);
	}

private:
	mutable FunctionsSet m_Functions;
};



class EventArgs
{
public:
	EventArgs(const Object& caller)
		: Caller(caller)
	{}

	// The object that invoked the event
	const Object& Caller;
};
typedef Delegate<EventArgs> Event;


class WindowCloseEventArgs : EventArgs
{
public:
	typedef EventArgs base;
	WindowCloseEventArgs(const Object& caller)
		: base(caller)
		, ConfirmClose(true)
	{}

	// The user can cancel a window closing operating by registering for the 
	// Window::Close event on the window and setting the ConfirmClose to false if
	// the window should be kept open (for example, there are unsaved changes 
	// made and closing the window would cause those changes to be lost).
	// By default, the window will be destoryed if the Window::Close even is not handled.
	bool ConfirmClose;
};
typedef Delegate<WindowCloseEventArgs> WindowCloseEvent;


class KeyEventArgs : public EventArgs
{
public:
	enum KeyState
	{
		Released = 0,
		Pressed = 1
	};

	typedef EventArgs base;
	KeyEventArgs(const Object& caller, KeyCode key, uint32 c, KeyState state, bool control, bool shift, bool alt)
		: base(caller)
		, Key(key)
		, Char(c)
		, State(state)
		, Control(control)
		, Shift(shift)
		, Alt(alt)
	{}

	KeyCode         Key;    // The Key Code that was pressed or released.
	uint32          Char;   // The 32-bit character code that was pressed. This value will be 0 if it is a non-printable character.
	KeyState        State;  // Was the key pressed or released?
	bool            Control;// Is the Control modifier pressed
	bool            Shift;  // Is the Shift modifier pressed
	bool            Alt;    // Is the Alt modifier pressed
};
typedef Delegate<KeyEventArgs> KeyboardEvent;



class MouseMotionEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	MouseMotionEventArgs(const Object& caller, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: base(caller)
		, LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Control(control)
		, Shift(shift)
		, X(x)
		, Y(y)
	{}

	bool LeftButton;    // Is the left mouse button down?
	bool MiddleButton;  // Is the middle mouse button down?
	bool RightButton;   // Is the right mouse button down?
	bool Control;       // Is the CTRL key down?
	bool Shift;         // Is the Shift key down?

	int X;              // The X-position of the cursor relative to the upper-left corner of the client area.
	int Y;              // The Y-position of the cursor relative to the upper-left corner of the client area.
	int RelX;			// How far the mouse moved since the last event.
	int RelY;			// How far the mouse moved since the last event.

};
typedef Delegate<MouseMotionEventArgs> MouseMotionEvent;



class MouseButtonEventArgs : public EventArgs
{
public:
	enum MouseButton
	{
		None = 0,
		Left = 1,
		Right = 2,
		Middel = 3
	};
	enum ButtonState
	{
		Released = 0,
		Pressed = 1
	};

	typedef EventArgs base;
	MouseButtonEventArgs(const Object& caller, MouseButton buttonID, ButtonState state, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: base(caller)
		, Button(buttonID)
		, State(state)
		, LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Control(control)
		, Shift(shift)
		, X(x)
		, Y(y)
	{}

	MouseButton Button; // The mouse button that was pressed or released.
	ButtonState State;  // Was the button pressed or released?
	bool LeftButton;    // Is the left mouse button down?
	bool MiddleButton;  // Is the middle mouse button down?
	bool RightButton;   // Is the right mouse button down?
	bool Control;       // Is the CTRL key down?
	bool Shift;         // Is the Shift key down?

	int X;              // The X-position of the cursor relative to the upper-left corner of the client area.
	int Y;              // The Y-position of the cursor relative to the upper-left corner of the client area.
};
typedef Delegate<MouseButtonEventArgs> MouseButtonEvent;



class MouseWheelEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	MouseWheelEventArgs(const Object& caller, float wheelDelta, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
		: base(caller)
		, WheelDelta(wheelDelta)
		, LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Control(control)
		, Shift(shift)
		, X(x)
		, Y(y)
	{}

	float WheelDelta;   // How much the mouse wheel has moved. A positive value indicates that the wheel was moved to the right. A negative value indicates the wheel was moved to the left.
	bool LeftButton;    // Is the left mouse button down?
	bool MiddleButton;  // Is the middle mouse button down?
	bool RightButton;   // Is the right mouse button down?
	bool Control;       // Is the CTRL key down?
	bool Shift;         // Is the Shift key down?

	int X;              // The X-position of the cursor relative to the upper-left corner of the client area.
	int Y;              // The Y-position of the cursor relative to the upper-left corner of the client area.

};
typedef Delegate<MouseWheelEventArgs> MouseWheelEvent;



class ResizeEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	ResizeEventArgs(const Object& caller, int width, int height)
		: base(caller)
		, Width(width)
		, Height(height)
	{}

	int Width;
	int Height;
};
typedef Delegate<ResizeEventArgs> ResizeEvent;