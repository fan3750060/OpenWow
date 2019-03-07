#pragma once

#include "UIBaseNode.h"
#include "Materials/UI_Color_Material.h"

class CUIColorNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUIColorNode();
	virtual ~CUIColorNode();

	// CUIColorNode
	void SetColor(vec4 _color);

	// CUIBaseNode
	virtual bool AcceptMesh(IVisitor& visitor) override;

private:
	std::shared_ptr<UI_Color_Material>  m_Material;
};