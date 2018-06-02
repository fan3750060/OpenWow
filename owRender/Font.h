#pragma once

struct R_GeometryInfo;
#include "TexturesManager.h"

class Font : public CRefItem
{
public:
	Font(R_Texture* _texture, R_GeometryInfo* _fontGeometry, vector<uint32> _widthArray, uint32 _height);
	~Font();

	void Render(cstring _string, vec2 _offset) const;

	uint32 GetStringWidth(cstring _string) const;
	uint32 GetHeight() const;

public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 192;

private:
	SmartTexturePtr	m_Texture;
	SmartGeomPtr	m_Geometry;
	vector<uint32>	m_WidthArray;
	uint32			m_Height;
};
