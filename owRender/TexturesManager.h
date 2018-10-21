#pragma once

class TexturesManager : public ITexturesManager, public CRefManager1DimAssync<R_Texture>
{
public:
	TexturesManager(IOpenGLAdapter* _adapter, RenderDevice* _RenderDevice);
	virtual ~TexturesManager();

	SharedTexturePtr DefaultTexture() { return m_DefaultTexture2DObj; }

	// ITexturesManager
	SharedTexturePtr Add(cstring name) { return CRefManager1DimAssync::Add(name); }
	bool Exists(cstring name) const { return CRefManager1DimAssync::Exists(name); }
	void Delete(cstring name) { CRefManager1DimAssync::Delete(name); }
	void Delete(SharedTexturePtr item) { CRefManager1DimAssync::Delete(item); }

	// CRefManager1Dim
    SharedTexturePtr CreateAction(cstring name) override;
	void LoadAction(std::string name, SharedTexturePtr& item) override;
	bool DeleteAction(cstring name) override;
	void MakeContext() override;

public:
	static void LoadBLPTexture(std::shared_ptr<IFile> _file, SharedTexturePtr _texture);

private:
	SharedTexturePtr	m_DefaultTexture2DObj;
	SharedTexturePtr	m_DefaultTexture3DObj;
	SharedTexturePtr	m_DefaultTextureCubeObj;

private:
	IOpenGLAdapter*		m_Adapter;
	RenderDevice*		m_RenderDevice;
};
