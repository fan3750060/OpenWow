#include "stdafx.h"

// Include
#include "UIMgr.h"

// General
#include "UIButton3t.h"

UIButton3t::UIButton3t(IUIMgr* _uimgr) :
	UIButton(_uimgr)
{
	for (uint32 i = 0; i < 4; i++)
	{
		buttonsImages[i] = nullptr;
	}
}

UIButton3t::~UIButton3t()
{
	int x = 0;
}

void UIButton3t::Init(cvec2 _position, cstring _textureName)
{
	base::Init(_position, nullptr);
}

void UIButton3t::OnRenderUI()
{
	m_SecondImage = nullptr;

	switch (buttonStatus)
	{
		case BUTTON_NORMAL:
		m_Image = buttonsImages[0];
		break;

		case BUTTON_PRESSED:
		m_Image = buttonsImages[1];
		break;

		case BUTTON_HOVER:
		m_Image = buttonsImages[0];
		m_SecondImage = buttonsImages[2];
		break;

		case BUTTON_DISABLED:
		m_Image = buttonsImages[3];
		break;
	}

	base::OnRenderUI();
}
