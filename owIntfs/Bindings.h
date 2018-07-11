#pragma once

__interface IObject
{};

__interface IInputListener : public IObject
{
	void OnMouseMoved(cvec2 _mousePos);
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos);
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos);
	bool OnMouseWheel(int _yoffset);
	bool OnKeyboardPressed(int _key, int _scancode, int _mods);
	bool OnKeyboardReleased(int _key, int _scancode, int _mods);
	bool OnCharInput(uint32 _char);
};

// FORWARD BEGIN
class CInput;
// FORWARD END

struct IUpdatable : public IObject
{
	virtual ~IUpdatable() = 0 {}

	virtual void Input(CInput* _input, double _time, double _dTime) = 0;
	virtual void Update(double _time, double _dTime) = 0;
};

struct IRenderable3D : public IObject
{
	virtual ~IRenderable3D() = 0 {}

	virtual bool PreRender3D() = 0;
	virtual void Render3D() = 0;
	virtual void PostRender3D() = 0;

	virtual void setVisible(bool _value) = 0;
	virtual bool isVisible() const = 0;
	virtual void setOpaque(bool _value) = 0;
	virtual bool isOpaque() const = 0;
	virtual void setDrawOrder(uint32 _order) = 0;
	virtual uint32 getDrawOrder() const = 0;
};

struct IRenderableUI : public IObject
{
	virtual ~IRenderableUI() = 0 {}

	virtual void RenderUI() = 0;
};