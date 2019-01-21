#include "stdafx.h"

// General
#include "UIText.h"

void UIText::SetText(const std::string& _text)
{
	m_Text = _text;
}

std::string UIText::GetText() const
{
	return m_Text;
}
