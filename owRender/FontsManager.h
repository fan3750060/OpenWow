#pragma once

#include "Font.h"

class FontsManager : public CRefManager1Dim<Font>, public IFontsManager
{
public:
	FontsManager(RenderDevice* _RenderDevice);
	virtual ~FontsManager() {}

	Font* Add(cstring _fontFileName, uint32 _fontSize);

	Font* GetMainFont() const { return mainFont; }

	// CRefManager1Dim
	Font* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

	// IFontsManager
	Font* Add(cstring name) { return CRefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return CRefManager1Dim::Exists(name); }
	void Delete(cstring name) { CRefManager1Dim::Delete(name); }
	void Delete(Font* item) { CRefManager1Dim::Delete(item); }

private:
	SharedFontPtr mainFont;
	RenderDevice* m_RenderDevice;
};
