#pragma once

#include "TextureLoader.h"

class TextureLoadersStorage
{
public:
	//bool RegisterTextureLoader(std::shared_ptr<TextureLoader> _loader);
	//void UnregisterTextureLoader(std::shared_ptr<TextureLoader> _loader);

	//TextureLoader* GetTextureLoader(cstring _fileName);

private:
	std::vector<std::shared_ptr<TextureLoader>> m_TextureLoaders;
};