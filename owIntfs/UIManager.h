#pragma once

// FORWARD BEGIN
class UIElement;
// FORWARD END

__interface 
	__declspec(uuid("30C0C1B9-6B0B-4D0B-A4DB-8AEB1F1FD6C3"))
	IUIMgr : public IManager
{
	void AttachToRoot(UIElement* _element);
	void DetachFromRoot(UIElement* _element, bool _checkChilds = false);

	void AttachElementToParent(UIElement* _element, UIElement* _parent);

	UIElement* GetRootElement() const;
	void SetRootElement(UIElement* _element);

	UIElement* GetFocus() const;
	void SetFocus(UIElement* _element);
};

__interface	IUIMgrEx
{
	string GetNewName();
	void SetForDetach(UIElement* _element);
	void SetForDelete(UIElement* _element);
	void DetachFromParent(UIElement* _element);
	void DeleteUIElement(UIElement* _element);
};