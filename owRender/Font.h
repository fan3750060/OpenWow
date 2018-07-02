#pragma once

struct R_GeometryInfo;
#include "TexturesManager.h"

class Font : public CRefItem
{
public:
	Font(R_Texture* _texture, R_GeometryInfo* _fontGeometry, vector<uint32> _widthArray, uint32 _height);
	~Font();

	void Render(cstring _string, vec2 _offset, const Color& _color) const;

	// Getters
	uint32 GetStringWidth(cstring _string) const;
	R_Texture* getTexture() const { return m_Texture; }
	R_GeometryInfo* getGeometry() const { return m_Geometry; }
	uint32 GetHeight() const { return m_Height; }

public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 256;

private:
	SmartTexturePtr	m_Texture;
	SmartGeomPtr	m_Geometry;
	vector<uint32>	m_WidthArray;
	uint32			m_Height;
};
