#pragma once

class TexturesManager : public ITexturesManager, public CRefManager1DimAssync<R_Texture>
{
public:
	TexturesManager(IOpenGLAdapter* _adapter, RenderDevice* _RenderDevice);
	virtual ~TexturesManager();

	std::shared_ptr<Texture> DefaultTexture() { return m_DefaultTexture2DObj; }

	// ITexturesManager
	std::shared_ptr<Texture> Add(cstring name) { return CRefManager1DimAssync::Add(name); }
	bool Exists(cstring name) const { return CRefManager1DimAssync::Exists(name); }
	void Delete(cstring name) { CRefManager1DimAssync::Delete(name); }
	void Delete(std::shared_ptr<Texture> item) { CRefManager1DimAssync::Delete(item); }

	// CRefManager1Dim
    std::shared_ptr<Texture> CreateAction(cstring name) override;
	void LoadAction(std::string name, std::shared_ptr<Texture>& item) override;
	bool DeleteAction(cstring name) override;
	void MakeContext() override;

public:
	static void LoadBLPTexture(std::shared_ptr<IFile> _file, std::shared_ptr<Texture> _texture);

private:
	std::shared_ptr<Texture>	m_DefaultTexture2DObj;
	std::shared_ptr<Texture>	m_DefaultTexture3DObj;
	std::shared_ptr<Texture>	m_DefaultTextureCubeObj;

private:
	IOpenGLAdapter*		m_Adapter;
	RenderDevice*		m_RenderDevice;
};
