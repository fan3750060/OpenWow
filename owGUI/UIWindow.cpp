#include "stdafx.h"

// Include
#include "UIMgr.h"

// General
#include "UIWindow.h"

UIWindow::UIWindow(IUIMgr* _uimgr) :
	UIElement(_uimgr)
{
	//color = COLOR_RED.Alpha(0.2);
}

UIWindow::~UIWindow()
{
}

void UIWindow::Init(cvec2 _position, cvec2 _size, Image* _image, Color _color)
{
	m_IsTextEnable = false;

	base::Init(_position, _size, _image, _color);
}

void UIWindow::AddContent(UIElement* _element)
{
    m_Content.push_back(_element);
    _element->AttachTo(this);
}
