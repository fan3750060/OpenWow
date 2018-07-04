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

__interface IUpdatable : public IObject
{
	void Input(CInput* _input, double _time, double _dTime);
	void Update(double _time, double _dTime);
};

__interface IRenderable3D : public IObject
{
	bool PreRender3D();
	void Render3D();
	void PostRender3D();

	void setVisible(bool _value);
	bool isVisible() const;
	void setOpaque(bool _value);
	bool isOpaque() const;
	void setDrawOrder(uint32 _order);
	uint32 getDrawOrder() const;
};

__interface IRenderableUI : public IObject
{
	void RenderUI();
};