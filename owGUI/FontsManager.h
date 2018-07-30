#pragma once

#include "Font.h"

class FontsManager : public CRefManager1Dim<Font>, public IFontsManager
{
public:
	FontsManager(RenderDevice* _RenderDevice);
	virtual ~FontsManager() {}

	SharedFontPtr Add(cstring _fontFileName, uint32 _fontSize);

	shared_ptr<Font> GetMainFont() const override { return mainFont; }

	// CRefManager1Dim
	SharedFontPtr CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

	// IFontsManager
	SharedFontPtr Add(cstring name) { return CRefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return CRefManager1Dim::Exists(name); }
	void Delete(cstring name) { CRefManager1Dim::Delete(name); }
	void Delete(SharedFontPtr item) { CRefManager1Dim::Delete(item); }

private:
	SharedFontPtr mainFont;

private:
	RenderDevice* m_RenderDevice;
};
