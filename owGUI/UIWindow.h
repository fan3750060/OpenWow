#pragma once

#include "UIElement.h"

class UIWindow : public UIElement
{
	friend UIMgr;
	typedef UIElement base;
public:
	UIWindow(IUIMgr* _uimgr);

	void Init(cvec2 _position, cvec2 _size, Image* _image, Color _color = COLOR_EMPTY);

    void AddContent(UIElement* _element);

private:
    vector<UIElement*> m_Content;
};
