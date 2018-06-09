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

__interface IUpdatable : public IObject
{
	void Input(double _time, double _dTime);
	void Update(double _time, double _dTime);
};

__interface IRenderable3D : public IObject
{
	void PreRender3D();
	void Render3D();
	void PostRender3D();

	bool IsVisible() const;
	void SetVisible(bool _value);
	void SetDrawOrder(uint32 _order);
	uint32 GetDrawOrder() const;
};

__interface IRenderableUI : public IObject
{
	void RenderUI();

	void SetDrawOrder(uint32 _order);
	uint32 GetDrawOrder() const;
};