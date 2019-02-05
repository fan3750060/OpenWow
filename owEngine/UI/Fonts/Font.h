#pragma once

// FORWARD BEGIN
class MeshWrapper;
class UI_Font_Material;
// FORWARD END

class Font : public MeshWrapper
{
public:
	Font(std::shared_ptr<Texture> _texture, std::shared_ptr<IMesh> _fontGeometry, std::vector<uint32> _widthArray, uint32 _height);
	~Font();

	// Getters
	bool Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, cstring _text);
	uint32 GetStringWidth(cstring _string) const;
	uint32 GetHeight() const;
	 
public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 256;

private:
	std::shared_ptr<UI_Font_Material> m_Material;
	std::shared_ptr<Texture>	m_Texture;
	std::shared_ptr<IMesh>	    m_Geometry;
	std::vector<uint32>	        m_WidthArray;
	uint32			            m_Height;
};

