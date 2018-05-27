#pragma once

class TexturesManager : public ITexturesManager, public RefManager1Dim<R_Texture>
{
public:
	TexturesManager(RenderDevice* _RenderDevice);
	~TexturesManager();

	R_Texture* DefaultTexture() { return m_DefaultTexture2DObj; }

	// ITexturesManager
	R_Texture* Add(cstring name) { return RefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return RefManager1Dim::Exists(name); }
	void Delete(cstring name) { RefManager1Dim::Delete(name); }
	void Delete(R_Texture* item) { RefManager1Dim::Delete(item); }

	// RefManager1Dim
    R_Texture* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

private:
	R_Texture* LoadBLPTexture(IFile* _file);

private:
	SmartTexturePtr	m_DefaultTexture2DObj;
	SmartTexturePtr	m_DefaultTexture3DObj;
	SmartTexturePtr	m_DefaultTextureCubeObj;

private:
	RenderDevice* m_RenderDevice;
};

