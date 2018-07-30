#pragma once

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>

#include "KeyCodes.h"

class Camera;
class PipelineState;

// Delegate template class for encapsulating event callback functions.
template< class ArgumentType >
class Delegate
{
public:
	typedef boost::function< void(ArgumentType&) > FunctionType;
	typedef boost::signals2::connection ConnectionType;

	// Using scoped connections can help to manage connection lifetimes.
	typedef std::vector< boost::signals2::scoped_connection > ScopedConnections;

	// Add a callback to the the list
	// Returns the connection object that can be used to disconnect the 
	// subscriber from the signal.
	ConnectionType operator += (typename const FunctionType& callback) const
	{
		return m_Callbacks.connect(callback);
	}

	// Remove a callback from the list
	void operator -= (typename const FunctionType& callback) const
	{
		assert(false);
		// TODO: This isn't working yet.. Getting a compiler error:
		// Error	1	error C2666: 'boost::operator ==' : 4 overloads have similar conversions signal_template.hpp
		// WORKAROUND: Use the connection object returned when the subscriber was initially connected
		// to disconnect the subscriber.
		m_Callbacks.disconnect<FunctionType>(callback);
	}

	void operator -= (ConnectionType& con)
	{
		m_Callbacks.disconnect(con); // This doesn't seem to work either!?
		if (con.connected())
		{
			con.disconnect(); // This is stupid, then any connection passed to this function will be disconnected, even if it was never registered with this signal.
		}
	}

	// Invoke this event with the argument
	void operator()(typename ArgumentType& argument)
	{
		m_Callbacks(argument);
	}

private:
	typedef boost::signals2::signal< void(ArgumentType&) > Callbacks;
	mutable Callbacks   m_Callbacks;
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
	uint32    Char;   // The 32-bit character code that was pressed. This value will be 0 if it is a non-printable character.
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



class UpdateEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	UpdateEventArgs(const Object& caller, float fDeltaTime, float fTotalTime)
		: base(caller)
		, ElapsedTime(fDeltaTime)
		, TotalTime(fTotalTime)
	{}

	float ElapsedTime;
	float TotalTime;
};
typedef Delegate<UpdateEventArgs> UpdateEvent;



class RenderEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	RenderEventArgs(const Object& caller, float fDeltaTime, float fTotalTime, uint64_t frameCounter, Camera* camera = nullptr, PipelineState* pipelineState = nullptr)
		: base(caller)
		, ElapsedTime(fDeltaTime)
		, TotalTime(fTotalTime)
		, FrameCounter(frameCounter)
		, Camera(camera)
		, PipelineState(pipelineState)
	{}

	float ElapsedTime;
	float TotalTime;
	int64_t FrameCounter;
	Camera* Camera;
	PipelineState* PipelineState;
};
typedef Delegate<RenderEventArgs> RenderEvent;



class UserEventArgs : public EventArgs
{
public:
	typedef EventArgs base;
	UserEventArgs(const Object& caller, int code, void* data1, void* data2)
		: base(caller)
		, Code(code)
		, Data1(data1)
		, Data2(data2)
	{}

	int     Code;
	void*   Data1;
	void*   Data2;
};
typedef Delegate<UserEventArgs> UserEvent;