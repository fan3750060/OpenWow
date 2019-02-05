#pragma once

// FORWARD BEGIN
class Image;
class UIElement;
// FORWARD END

struct
	__declspec(novtable, uuid("30C0C1B9-6B0B-4D0B-A4DB-8AEB1F1FD6C3"))
	IUIMgr : public IManager
{
	virtual ~IUIMgr() = 0 {};

	virtual void AttachToRoot(UIElement* _element) = 0;
	virtual void DetachFromRoot(UIElement* _element, bool _checkChilds = false) = 0;

	virtual void AttachElementToParent(UIElement* _element, UIElement* _parent) = 0;

	virtual UIElement* GetRootElement() const = 0;
	virtual void SetRootElement(UIElement* _element) = 0;

	virtual UIElement* GetFocus() const = 0;
	virtual void SetFocus(UIElement* _element) = 0;

	virtual void RenderImage(vec2 _pos, Image* _image) = 0;
	virtual void RenderImage(vec2 _pos, Image* _image, vec2 _size) = 0;

	virtual void RenderText(vec2 _pos, cstring _string, const Color& _color) const = 0;
	virtual void RenderText(vec2 _pos, cstring _string, std::shared_ptr<Font> _font, const Color& _color) const = 0;
	virtual void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color) const = 0;
	virtual void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, std::shared_ptr<Font> _font, const Color& _color) const = 0;
};

struct	IUIMgrEx
{
	std::string GetNewName();
	void SetForDetach(UIElement* _element);
	void SetForDelete(UIElement* _element);
	void DetachFromParent(UIElement* _element);
	void DeleteUIElement(UIElement* _element);
};