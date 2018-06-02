#pragma once

#include "Font.h"

class FontsManager : public IFontsManager, public RefManager1Dim<Font>
{
public:
	FontsManager(RenderDevice* _RenderDevice);

	Font* Add(cstring _fontFileName, uint32 _fontSize);

	Font* GetMainFont() const { return mainFont; }

	// IFontsManager
	Font* Add(cstring name) { return RefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return RefManager1Dim::Exists(name); }
	void Delete(cstring name) { RefManager1Dim::Delete(name); }
	void Delete(Font* item) { RefManager1Dim::Delete(item); }

	// RefManager1Dim
	Font* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

private:
	SmartPtr<Font> mainFont;
	RenderDevice* m_RenderDevice;
};
