#pragma once

struct R_GeometryInfo;

class Font : public RefItem
{
public:
	Font(R_Texture* _Texture, R_GeometryInfo* _fontGeom, uint32* charWidthArray, uint32 charHeight);

	 ~Font();

	 void Render(cstring _string, vec2 _offset) const;

	 uint32 GetStringWidth(cstring _string) const;
	 uint32 GetHeight() const;

public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 192;

private:
	R_Texture* m_Texture;
    R_GeometryInfo* m_FontBuffer;
	uint32* m_CharWidthArray;
	uint32 m_CharHeight;
};
