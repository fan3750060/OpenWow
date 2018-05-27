#pragma once

class TexturesManager : public RefManager1Dim<R_Texture>
{
public:
	TexturesManager(RenderDevice* _RenderDevice);
	~TexturesManager();

    R_Texture* Add(cstring _textureFileName);
    R_Texture* Add(IFile* _textureFile);

    R_Texture* DefaultTexture() { return m_DefaultTexture2DObj; }

protected:
    R_Texture* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

public:
	bool LoadBLPTexture(IFile* _file, R_Texture*& _texture);

private:
    R_Texture*	m_DefaultTexture2DObj;
	R_Texture*	m_DefaultTexture3DObj;
	R_Texture*	m_DefaultTextureCubeObj;

private:
	RenderDevice* m_RenderDevice;
};
