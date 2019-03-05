#pragma once

#include "Font.h"

class FontsManager : public CRefManager1Dim<CFontMesh>, public IFontsManager
{
public:
	FontsManager();
	virtual ~FontsManager();

	std::shared_ptr<CFontMesh> Add(cstring _fontFileName, uint32 _fontSize);

	
	std::shared_ptr<CFontMesh> GetMainFont() const override { return mainFont; }

	// CRefManager1Dim
	std::shared_ptr<CFontMesh> CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

	// IFontsManager
	std::shared_ptr<CFontMesh> Add(cstring name) { return CRefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return CRefManager1Dim::Exists(name); }
	void Delete(cstring name) { CRefManager1Dim::Delete(name); }
	void Delete(std::shared_ptr<CFontMesh> item) { CRefManager1Dim::Delete(item); }

private:
	std::shared_ptr<CFontMesh> mainFont;
};
