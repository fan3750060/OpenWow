#pragma once

class TexturesManager : public ITexturesManager, public CRefManager1Dim<R_Texture>
{
public:
	TexturesManager(RenderDevice* _RenderDevice);

	R_Texture* DefaultTexture() { return m_DefaultTexture2DObj; }

	// ITexturesManager
	R_Texture* Add(cstring name) { return CRefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return CRefManager1Dim::Exists(name); }
	void Delete(cstring name) { CRefManager1Dim::Delete(name); }
	void Delete(R_Texture* item) { CRefManager1Dim::Delete(item); }

	// CRefManager1Dim
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

