#pragma once

class Font 
{
public:
	Font(std::shared_ptr<Texture> _texture, std::shared_ptr<IMesh> _fontGeometry, std::vector<uint32> _widthArray, uint32 _height);
	~Font();

	void Render(cstring _string, vec2 _offset, const Color& _color) const;

	// Getters
	uint32 GetStringWidth(cstring _string) const;
	std::shared_ptr<Texture> getTexture() const { return m_Texture; }
	std::shared_ptr<IMesh> getGeometry() const { return m_Geometry; }
	uint32 GetHeight() const { return m_Height; }

public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 256;

private:
	std::shared_ptr<Texture>	m_Texture;
	std::shared_ptr<IMesh>	m_Geometry;
	std::vector<uint32>	m_WidthArray;
	uint32			m_Height;
};

/*struct FontDeleter
{
	void operator()(Font* p)
	{
		IFontsManager* fontManager = GetManager<IFontsManager>();
		if (fontManager != nullptr)
		{
			fontManager->Delete(p);
		}
	}
};*/
typedef std::shared_ptr<Font> std::shared_ptr<Font>;
