#pragma once

class RenderDevice;

class R_Texture : public CRefItem
{
public:
	R_Texture(RenderDevice* _RenderDevice) :
		glObj(0),
		glFmt(0),
		type(0),
		format(R_TextureFormats::Unknown),
		width(0),
		height(0),
		depth(0),
		memSize(0),
		samplerState(0),
		sRGB(false),
		hasMips(false),
		genMips(false),
		m_RenderDevice(_RenderDevice)
	{}

	R_Texture* createTexture(R_TextureTypes::List type, int width, int height, int depth, R_TextureFormats::List format, bool hasMips, bool genMips, bool compress, bool sRGB);
	void uploadTextureData(int slice, int mipLevel, const void *pixels);
	void destroyTexture();
	bool getTextureData(int slice, int mipLevel, void *buffer);
	void bindImageToTexture(void* eglImage);

	inline vec2 GetSize()
	{
		return vec2(static_cast<float>(width), static_cast<float>(height));
	}

public: // Static
	static uint32 calcTextureSize(R_TextureFormats::List format, int width, int height, int depth);

public:
	uint32                  glObj;
	uint32                  glFmt;
	int                     type;
	R_TextureFormats::List  format;
	int                     width, height, depth;
	int                     memSize;
	uint32                  samplerState;
	bool                    sRGB;
	bool                    hasMips, genMips;

protected:
	RenderDevice*           m_RenderDevice;
};

struct R_TextureDeleter
{
	void operator()(R_Texture* p)
	{
		p->destroyTexture();
		GetManager<ITexturesManager>()->Delete(p);
	}
};
typedef SmartPtr<R_Texture, R_TextureDeleter> SmartTexturePtr;