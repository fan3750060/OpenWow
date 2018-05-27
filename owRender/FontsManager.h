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

protected:
	Font* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

private:
	SmartPtr<Font> mainFont;

private:
	RenderDevice* m_RenderDevice;
};
