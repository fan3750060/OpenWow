#include "stdafx.h"

// General
#include "UIColor.h"

// Additional
#include "Application.h"

CUIColorNode::CUIColorNode(vec2 Size)
	: base(Size)
{
	m_Material = std::make_shared<UI_Color_Material>();
	m_Material->SetWrapper(m_Material);;

	m_Mesh = _RenderDevice->CreateUIQuad(Size.x, Size.y);
}

CUIColorNode::~CUIColorNode()
{}



//
// CUIColorNode
//
void CUIColorNode::SetColor(vec4 _color)
{
	m_Material->SetColor(_color);
}


//
// CUIBaseNode
//
bool CUIColorNode::AcceptMesh(IVisitor& visitor)
{
	m_Mesh->SetMaterial(m_Material);

	return m_Mesh->Accept(visitor);
}
