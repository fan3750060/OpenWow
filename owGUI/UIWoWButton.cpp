#include "stdafx.h"

// General
#include "UIWoWButton.h"

UIWowButon::UIWowButon(IUIMgr* _uimgr) :
	UIButton3t(_uimgr)
{}

UIWowButon::~UIWowButon()
{
	for (uint32 i = 0; i < 4; i++)
	{
		if (buttonsImages[i] != nullptr)
		{
			delete buttonsImages[i];
			buttonsImages[i] = nullptr;
		}
	}
}

void UIWowButon::Init(cvec2 _position)
{
	vec2 size = vec2(128, 22);

	buttonsImages[0] = new Image(_Render->TexturesMgr()->Add("Interface\\Buttons\\UI-DialogBox-Button-Up.blp"), vec2(), size);
	buttonsImages[1] = new Image(_Render->TexturesMgr()->Add("Interface\\Buttons\\UI-DialogBox-Button-Down.blp"), vec2(), size);
	buttonsImages[2] = new Image(_Render->TexturesMgr()->Add("Interface\\Buttons\\UI-DialogBox-Button-Highlight.blp"), vec2(), size);
	buttonsImages[3] = new Image(_Render->TexturesMgr()->Add("Interface\\Buttons\\UI-DialogBox-Button-Disabled.blp"), vec2(), size);

	UIButton::Init(_position, buttonsImages[0]);
}
