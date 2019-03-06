#include "stdafx.h"

// General
#include "UIColor.h"

// Additional
#include "Application.h"

CUIColorNode::CUIColorNode()
{
	m_Material = std::make_shared<UI_Color_Material>();
	m_Material->SetWrapper(m_Material);;

	std::shared_ptr<IMesh> mesh = _RenderDevice->CreateScreenQuad();
	SetMesh(mesh);
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
	GetMesh()->SetMaterial(m_Material);

	return GetMesh()->Accept(visitor);
}
