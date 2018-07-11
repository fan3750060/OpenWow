#pragma once

#include "TexturesManager.h"

// FORWARD BEGIN
class R_GeometryInfo;
// FORWARD END

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
	SharedTexturePtr	m_Texture;
	SharedGeomPtr	m_Geometry;
	vector<uint32>	m_WidthArray;
	uint32			m_Height;
};

struct FontDeleter
{
	void operator()(Font* p)
	{
		IFontsManager* fontManager = GetManager<IFontsManager>();
		if (fontManager != nullptr)
		{
			fontManager->Delete(p);
		}
	}
};
typedef SharedPtr<Font, FontDeleter> SharedFontPtr;