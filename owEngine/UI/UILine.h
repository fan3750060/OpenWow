#pragma once

#include "UIBaseNode.h"
#include "Materials/UI_Line_Material.h"

class CUILineNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
    CUILineNode(vec2 Size = vec2(1.0f, 1.0f));
	virtual ~CUILineNode();

	// CUILineNode
	void SetColor(vec4 Color);
    void SetPosStart(vec2 PosStart);
    void SetPosEnd(vec2 PosEnd);
    void SetThickness(float Thickness);

	// CUIBaseNode
	virtual bool AcceptMesh(IVisitor& visitor) override;

private:
	std::shared_ptr<IMesh>              m_Mesh;
	std::shared_ptr<UI_Line_Material>  m_Material;
};