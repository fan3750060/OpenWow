#pragma once

class MeshWrapper;
class UI_Font_Material;

class Font : public MeshWrapper
{
public:
	Font(std::shared_ptr<Texture> _texture, std::shared_ptr<IMesh> _fontGeometry, std::vector<uint32> _widthArray, uint32 _height);
	~Font();

	// MeshWrapper
	bool Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt) override;

	// Getters
	uint32 GetStringWidth(cstring _string) const;
	std::shared_ptr<Texture> getTexture() const { return m_Texture; }
	std::shared_ptr<IMesh> getGeometry() const { return m_Geometry; }
	uint32 GetHeight() const { return m_Height; }

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
