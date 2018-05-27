#pragma once

#include "Font.h"

class FontsManager : public RefManager1Dim<Font>
{
public:
	FontsManager(RenderDevice* _RenderDevice);
	~FontsManager();

	Font* Add(cstring _fontFileName, uint32 _fontSize);
	Font* Add(IFile* _fontFile, uint32 _fontSize);

	//
	
	Font* GetMainFont() const { return mainFont; }
	uint32_t GetFontsCount() const { return Fonts.size(); }

protected:
	Font* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

private:
	map<string, Font*> Fonts;

	Font* mainFont;

private:
	RenderDevice* m_RenderDevice;
};
