#pragma once

#include <iostream>

class Module;
class File;

class TexturesManager : public RefManager1DimAssync<R_Texture>
{
public:
	TexturesManager(RenderDevice* _RenderDevice);
	~TexturesManager();

    R_Texture* Add(cstring _textureFileName);
    R_Texture* Add(File& _textureFile);

    R_Texture* DefaultTexture() { return m_DefaultTexture2DObj; }

protected:
    R_Texture* CreateAction(cstring name) override;
	void LoadAction(string _name, R_Texture*& _texture) override;
	bool DeleteAction(cstring name) override;

public:
	bool LoadBLPTexture(File& _file, R_Texture*& _texture);

private:
    R_Texture*   m_DefaultTexture2DObj;
    R_Texture*   m_DefaultTexture3DObj;
    R_Texture*   m_DefaultTextureCubeObj;

	ContainerAssync<string, R_Texture*> textures;

private:
	RenderDevice* m_RenderDevice;
};
