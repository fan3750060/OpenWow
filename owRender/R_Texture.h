#pragma once

class RenderDevice;

class R_Texture : public CRefItem
{
public:
	R_Texture(cstring _name, RenderDevice* _renderDevice);
	~R_Texture();

	void FillDataBy(R_Texture* _other)
	{
		m_GLObj = _other->m_GLObj;
		m_GLFmt = _other->m_GLFmt;
		m_Type = _other->m_Type;
		m_Format = _other->m_Format;
		m_Width = _other->m_Width;
		m_Height = _other->m_Height;
		m_Depth = _other->m_Depth;
		m_MemSize = _other->m_MemSize;
		m_SamplerState = _other->m_SamplerState;
		m_IssRGB = _other->m_IssRGB;
		m_HasMips = _other->m_HasMips;
		m_GenMips = _other->m_GenMips;
	}

	R_Texture* createTexture(R_TextureTypes::List _type, int _width, int _height, int _depth, R_TextureFormats::List _format, bool _hasMips, bool _genMips, bool _compress, bool _sRGB);
	void uploadTextureData(int _slice, int _mipLevel, const void* _pixels);
	bool getTextureData(int _slice, int _mipLevel, void* _buffer);
	void bindImageToTexture(void* _eglImage);

	inline vec2 GetSize() { return vec2(static_cast<float>(m_Width), static_cast<float>(m_Height)); }

public: // Static
	static uint32 calcTextureSize(R_TextureFormats::List _format, int _width, int _height, int _depth);

public:
	string					m_Name;
	uint32                  m_GLObj;
	uint32                  m_GLFmt;
	int                     m_Type;
	R_TextureFormats::List  m_Format;
	int                     m_Width, m_Height, m_Depth;
	int                     m_MemSize;
	uint32                  m_SamplerState;
	bool                    m_IssRGB;
	bool                    m_HasMips;
	bool                    m_GenMips;

private:
	RenderDevice*			m_RenderDevice;
};

struct R_TextureDeleter
{
	void operator()(R_Texture* p)
	{
		ITexturesManager* textManager = GetManager<ITexturesManager>();
		if (textManager != nullptr)
		{
			textManager->Delete(p);
		}
	}
};
typedef SmartPtr<R_Texture, R_TextureDeleter> SmartTexturePtr;