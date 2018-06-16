#pragma once

class TexturesManager : public ITexturesManager, public CRefManager1DimAssync<R_Texture>
{
public:
	TexturesManager(IOpenGLAdapter* _adapter, RenderDevice* _RenderDevice);

	R_Texture* DefaultTexture() { return m_DefaultTexture2DObj; }

	// ITexturesManager
	R_Texture* Add(cstring name) { return CRefManager1DimAssync::Add(name); }
	bool Exists(cstring name) const { return CRefManager1DimAssync::Exists(name); }
	void Delete(cstring name) { CRefManager1DimAssync::Delete(name); }
	void Delete(R_Texture* item) { CRefManager1DimAssync::Delete(item); }

	// CRefManager1Dim
    R_Texture* CreateAction(cstring name) override;
	void LoadAction(string name, R_Texture*& item) override;
	bool DeleteAction(cstring name) override;
	void MakeContext() override;

public:
	static R_Texture* LoadBLPTexture(IFile* _file, R_Texture* _texture);

private:
	SmartTexturePtr	m_DefaultTexture2DObj;
	SmartTexturePtr	m_DefaultTexture3DObj;
	SmartTexturePtr	m_DefaultTextureCubeObj;

private:
	IOpenGLAdapter* m_Adapter;
	RenderDevice* m_RenderDevice;
};

