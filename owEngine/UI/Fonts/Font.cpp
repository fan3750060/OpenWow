#include <stdafx.h>

// Include 
#include "UI_Font_Material.h"

// General
#include "Font.h"

Font::Font(std::shared_ptr<Texture> _texture, std::shared_ptr<IMesh> _fontGeometry, std::vector<uint32> _widthArray, uint32 _height) 
	: MeshWrapper(SceneNodeTypes::SN_TYPE_NONE, _fontGeometry)
	, m_Texture(_texture)
	, m_WidthArray(_widthArray)
	, m_Height(_height)
{
	m_Material = std::make_shared<UI_Font_Material>();
	m_Material->SetTexture(0, m_Texture);

	SetMaterial(m_Material);
}

Font::~Font()
{
	Log::Info("Fonts deleted!!!");
}

bool Font::Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	std::string _string = "Hello, world!!";
	vec2 _offset = vec2(0.0f, 0.0f);

	for (uint32 i = 0; i < _string.length(); i++)
	{
		uint8 ch = _string.c_str()[i];
		m_Material->SetOffset(_offset);
		_offset.x += static_cast<float>(m_WidthArray[ch]);

		MeshWrapper::Render(renderEventArgs, perObject, indexStartLocation, indexCnt, (ch) * 6, 6);
	}

	return true;
}

uint32 Font::GetStringWidth(cstring _string) const
{
	uint32 width = 0;

	for (uint32 i = 0; i < _string.length(); i++)
	{
		uint8 ch = _string.c_str()[i];
		width += m_WidthArray[ch];
	}

	return width;
}