#include "stdafx.h"

// General
#include "Font.h"

// Additional
#include "TechniquesManager.h"

Font::Font(R_Texture* _texture, R_GeometryInfo* _fontGeometry, vector<uint32> _widthArray, uint32 _height) :
	RefItem(),
	m_Texture(_texture),
	m_Geometry(_fontGeometry),
	m_WidthArray(_widthArray),
	m_Height(_height)
{}

Font::~Font()
{
	m_Geometry->destroyGeometry(true);
}

void Font::Render(cstring _string, vec2 _offset) const
{
	_Render->r.setTexture(10, m_Texture, SS_FILTER_BILINEAR | SS_ADDR_CLAMP, 0);
	_Render->r.setGeometry(m_Geometry);
	
	for (uint32 i = 0; i < _string.length(); i++)
	{
		uint8 ch = _string.c_str()[i];
		if (static_cast<uint32>(ch) < SPACE || static_cast<uint32>(ch) - 32 > Font::NUM_CHARS)
		{
			continue;
		}

		_Render->TechniquesMgr()->m_UI_Font->SetCharOffset(_offset);
		_offset.x += static_cast<float>(m_WidthArray[ch - SPACE]);

		_Render->r.draw(PRIM_TRILIST, (ch - SPACE) * 6, 6);
	}
}

uint32 Font::GetStringWidth(cstring _string) const
{
	uint32 width = 0;

	for (auto it : _string)
	{
		if (it < 32)
		{
			continue;
		}

		width += m_WidthArray[it - SPACE];
	}

	return width;
}

uint32 Font::GetHeight() const
{
	return m_Height;
}