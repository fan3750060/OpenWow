#pragma once

class CInputListener : public IInputListener
{
public:
	CInputListener(IInputListener* _inputListener, uint8 _priority) :
		m_InputListener(_inputListener),
		m_Priority(_priority)
	{}

	CInputListener& operator=(const CInputListener& _other)
	{
		this->m_InputListener = _other.m_InputListener;
		this->m_Priority = _other.m_Priority;
		return *this;
	}

	void OnMouseMoved(cvec2 _mousePos) { m_InputListener->OnMouseMoved(_mousePos); }
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) { return m_InputListener->OnMouseButtonPressed(_button, _mods, _mousePos); }
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) { return m_InputListener->OnMouseButtonReleased(_button, _mods, _mousePos); }
	bool OnMouseWheel(int _yoffset) { return m_InputListener->OnMouseWheel(_yoffset); }
	bool OnKeyboardPressed(int _key, int _scancode, int _mods) { return m_InputListener->OnKeyboardPressed(_key, _scancode, _mods); }
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) { return m_InputListener->OnKeyboardReleased(_key, _scancode, _mods); }
	bool OnCharInput(uint32 _char) { return m_InputListener->OnCharInput(_char); }

	IInputListener* getInputListener() { return m_InputListener; }
	uint8 getPriority() const { return m_Priority; }

private:
	IInputListener* m_InputListener;
	uint8		m_Priority;
};

struct CInputListenerObjectCompare
{
	bool operator() (const CInputListener& left, const CInputListener& right) const
	{
		return left.getPriority() < right.getPriority();
	}
};

class CInputListenerObjectCollection : public IInputListener
{
public:
	~CInputListenerObjectCollection();

    bool RegisterObject(IInputListener* _object, uint8 _priority = 0);
    void UnregisterObject(IInputListener* _object);

    // IInputListener
    void OnMouseMoved(cvec2 _mousePos) override;
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseWheel(int _yoffset) override;
	bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override;
	bool OnCharInput(uint32 _char) override;

private:
    vector<CInputListener> m_Objects;
};