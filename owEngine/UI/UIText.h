#pragma once

#include "UIBaseNode.h"
#include "Fonts/Font.h"
#include "Materials/UI_Font_Material.h"

class CUITextNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUITextNode();
	virtual ~CUITextNode();

	// CUITextNode
	void                        SetFont(std::shared_ptr<CFontMesh> _font);
	void                        SetText(const std::string& _text);
	void                        SetColor(cvec4 _color);

	// CUIBaseNode
	void                        SetMesh(std::shared_ptr<IMesh> mesh) override;
	std::shared_ptr<IMesh>      GetMesh() const override;

	bool                        AcceptMesh(IVisitor& visitor) override;

private:
	std::shared_ptr<CFontMesh>         m_Font;
	std::shared_ptr<UI_Font_Material>  m_Material;
	std::string                        m_Text;
	glm::vec2                          m_Offset;
};