#pragma once

class UIElement;

class UIMgr : public IUpdatable, public CRenderableUIObject, public IInputListener
{
	CLASS_INSTANCE(UIMgr);
public:
	UIMgr();
	~UIMgr();
	//

	void AttachToRoot(UIElement* _element);
    void DetachFromRoot(UIElement* _element, bool _checkChilds = false);

    void AttachElementToParent(UIElement* _element, UIElement* _parent);

    // Root element

    UIElement* GetRootElement() const { return m_RootElement; }
    void SetRootElement(UIElement* _element) { m_RootElement = _element; }

	// Focus

	UIElement* GetFocus() const { return m_FocusedElement; }
    void SetFocus(UIElement* _element);

	// IUpdatable
	void Input(double _time, double _dTime) override {}
	void Update(double t, double dt) override;

	// IRenderableUI
	void RenderUI() override;

	// IInputListener
	void OnMouseMoved(cvec2 _mousePos) override;
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseWheel(int _yoffset) override;
	bool OnKeyboardPressed(int _key, int _scancode, int _mods) override;
	bool OnKeyboardReleased(int _key, int _scancode, int _mods) override;
	bool OnCharInput(uint32 _char) override;

private:
	string GetNewName();
	void SetForDetach(UIElement* _element);
	void SetForDelete(UIElement* _element);
	void DetachFromParent(UIElement* _element);
	void DeleteUIElement(UIElement* _element);

private:
	uint32              m_IDCounter;
    UIElement*          m_RootElement;
    UIElement*          m_FocusedElement;

	vector<UIElement*>  m_ObjectsToDetach;
	vector<UIElement*>  m_ObjectsToDelete;
	
    //

    friend UIElement;
};

#define _UIMgr UIMgr::instance()
