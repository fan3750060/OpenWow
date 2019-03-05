#include "stdafx.h"

// General
#include "UITexture.h"

// Additional
#include "Application.h"

namespace
{
	const vec4 cDefaultColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUITextureNode::CUITextureNode()
{
	m_Material = std::make_shared<UI_Texture_Material>();
	m_Material->SetWrapper(m_Material);;
	m_Material->SetColor(cDefaultColor);

	std::shared_ptr<IMesh> mesh = _RenderDevice->CreateScreenQuad();
	SetMesh(mesh);
}

CUITextureNode::~CUITextureNode()
{}



//
// CUITextureNode
//
void CUITextureNode::SetTexture(std::shared_ptr<Texture> _texture)
{
	m_Material->SetTexture(_texture);
}

void CUITextureNode::SetColor(vec4 _color)
{
	m_Material->SetColor(_color);
}


//
// CUIBaseNode
//
bool CUITextureNode::RenderMesh(RenderUIEventArgs& renderEventArgs)
{
	GetMesh()->SetMaterial(m_Material);

	return GetMesh()->Render(renderEventArgs, m_PerObjectConstantBuffer);
}
