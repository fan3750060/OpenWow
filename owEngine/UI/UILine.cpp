#include "stdafx.h"

// General
#include "UILine.h"

// Additional
#include "Application.h"

CUILineNode::CUILineNode(vec2 Size)
	: base(Size)
{
	m_Material = std::make_shared<UI_Line_Material>();
	m_Material->SetWrapper(m_Material);;

	m_Mesh = _RenderDevice->CreateBeizerLine(vec2(100, 200), vec2(1000, 1000));
}

CUILineNode::~CUILineNode()
{}



//
// CUILineNode
//
void CUILineNode::SetColor(vec4 Color)
{
	m_Material->SetColor(Color);
}

void CUILineNode::SetPosStart(vec2 PosStart)
{
    m_Material->SetPosStart(PosStart);
}

void CUILineNode::SetPosEnd(vec2 PosEnd)
{
    m_Material->SetPosEnd(PosEnd);
}

void CUILineNode::SetThickness(float Thickness)
{
    m_Material->SetThickness(Thickness);
}


//
// CUIBaseNode
//
bool CUILineNode::AcceptMesh(IVisitor& visitor)
{
	m_Mesh->SetMaterial(m_Material);

	return m_Mesh->Accept(visitor);
}
