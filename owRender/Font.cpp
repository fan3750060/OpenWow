#include "stdafx.h"

// General
#include "Font.h"

// Additional
#include "Material.h"
#include "Render.h"
#include "TechniquesManager.h"

Font::Font(R_Texture* _texture, R_GeometryInfo* _fontGeometry, vector<uint32> _widthArray, uint32 _height) :
	CRefItem(),
	m_Texture(_texture),
	m_Geometry(_fontGeometry),
	m_WidthArray(_widthArray),
	m_Height(_height)
{}

Font::~Font()
{
	Log::Info("Fonts deleted!!!");
}

void Font::Render(cstring _string, vec2 _offset, const Color& _color) const
{
	CUI_Font* pass = _Render->getTechniquesMgr()->UI_Font;
	pass->Bind();
	pass->SetProjectionMatrix(_Render->getOrthoMatrix());
	pass->SetFontColor(vec3(_color.red, _color.green, _color.blue));

	_Render->r.setTexture(Material::C_DiffuseTextureIndex, m_Texture, SS_FILTER_BILINEAR | SS_ADDR_CLAMP, 0);
	_Render->r.setGeometry(m_Geometry);
	
	for (uint32 i = 0; i < _string.length(); i++)
	{
		uint8 ch = _string.c_str()[i];
		if (static_cast<uint32>(ch) >= Font::NUM_CHARS)
		{
			continue;
		}

		pass->SetCharOffset(_offset);
		_offset.x += static_cast<float>(m_WidthArray[ch]);

		_Render->r.draw(PRIM_TRILIST, (ch) * 6, 6);
	}

	pass->Unbind();
}

uint32 Font::GetStringWidth(cstring _string) const
{
	uint32 width = 0;

	for (auto& it : _string)
	{
		if (it < 32)
		{
			continue;
		}

		width += m_WidthArray[it];
	}

	return width;
}