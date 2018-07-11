#pragma once

class CDebugOutput : public CRefItem, public IDebugOutput
{
public:
	virtual ~CDebugOutput()	{}

	// IRefItem
	void AddRef() { CRefItem::AddRef(); }
	void Release() { CRefItem::Release(); }
	uint32 GetRefsCount() const { return CRefItem::GetRefsCount(); }

	// IDebugOutput
	void PushMessage(cstring _message, IDebugOutput::DebugMessageType _type) override;
	void PushMessage(IDebugOutput::DebugMessageType _type, const char* _message, va_list& _vaList) override;

protected:
	virtual void Print(string _messageFmt, CDebugOutput::DebugMessageType _type) = 0;
};
