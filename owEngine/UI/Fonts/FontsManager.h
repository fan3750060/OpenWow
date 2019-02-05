#pragma once

#include "Font.h"

class FontsManager : public CRefManager1Dim<Font>, public IFontsManager
{
public:
	FontsManager();
	virtual ~FontsManager();

	std::shared_ptr<Font> Add(cstring _fontFileName, uint32 _fontSize);

	std::shared_ptr<Font> GetMainFont() const override { return mainFont; }

	// CRefManager1Dim
	std::shared_ptr<Font> CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

	// IFontsManager
	std::shared_ptr<Font> Add(cstring name) { return CRefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return CRefManager1Dim::Exists(name); }
	void Delete(cstring name) { CRefManager1Dim::Delete(name); }
	void Delete(std::shared_ptr<Font> item) { CRefManager1Dim::Delete(item); }

private:
	std::shared_ptr<Font> mainFont;

private:
	std::weak_ptr<IRenderDevice> m_RenderDevice;
};
