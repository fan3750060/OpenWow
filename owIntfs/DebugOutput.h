#pragma once

__interface 
	__declspec(uuid("DBB1D38E-CE3F-4C05-8625-313BFC524C5A"))
	IDebugOutput
{
	typedef enum DebugMessageType
	{
		TYPE_INFO = 0,
		TYPE_PRINT,
		TYPE_GREEN,
		TYPE_WARNING,
		TYPE_ERROR,
		TYPE_FATAL
	};

	void PushMessage(cstring _message, IDebugOutput::DebugMessageType _type);
	void PushMessage(IDebugOutput::DebugMessageType _type, const char* _message, va_list& _vaList);
};