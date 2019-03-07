#include "stdafx.h"

// General
#include "UIButton.h"

// Additional
#include "Application.h"

CUIButtonNode::CUIButtonNode()
	: m_State(Idle)
{
	m_Material = std::make_shared<UI_Button_Material>();
	m_Material->SetWrapper(m_Material);


}

CUIButtonNode::~CUIButtonNode()
{
}



//
// CUIButtonNode
//
void CUIButtonNode::CreateDefault()
{
	std::shared_ptr<Texture> idleTexture = _RenderDevice->CreateTexture2D("Textures\\btn_idle.png");

	m_Material->SetIdleTexture(idleTexture);
	m_Material->SetHoverTexture(_RenderDevice->CreateTexture2D("Textures\\btn_hover.png"));
	m_Material->SetClickedTexture(_RenderDevice->CreateTexture2D("Textures\\btn_clicked.png")); 
	m_Material->SetDisabledTexture(_RenderDevice->CreateTexture2D("Textures\\btn_disabled.png"));

	SetSize(glm::ivec2(idleTexture->GetWidth(), idleTexture->GetHeight()));

	std::shared_ptr<IMesh> mesh = _RenderDevice->CreateScreenQuad(0.0, idleTexture->GetWidth(), 0.0f, idleTexture->GetHeight());
	SetMesh(mesh);

	m_TextNode = std::make_shared<CUITextNode>();
	m_TextNode->SetParentInternal(weak_from_this());
	m_TextNode->SetText("Hello btn!");
	m_TextNode->SetTranslate(vec2(10.0f, 10.0f));
	m_TextNode->SetColor(vec4(0.0f, 0.0f, 1.0f, 1.0f));
}



//
// Input events
//
bool CUIButtonNode::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	m_State = Clicked;

	// Raise event
	UIButtonClickEventArgs args;
	OnClick(args);

	return true;
}

void CUIButtonNode::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (IsPointInBounds(e.GetPoint()))
	{
		m_State = Hover;
		return;
	}

	m_State = Idle;
}



//
// Syntetic events
//
void CUIButtonNode::OnMouseEntered()
{
	m_State = Hover;
}

void CUIButtonNode::OnMouseLeaved()
{
	m_State = Idle;
}





//
// CUIBaseNode
//
bool CUIButtonNode::Accept(IVisitor & visitor)
{
	bool visitResult = base::Accept(visitor);
	if (!visitResult)
		return false;

	if (m_TextNode != nullptr)
	{
		m_TextNode->Accept(visitor);
	}

	return true;
}

bool CUIButtonNode::AcceptMesh(IVisitor& visitor)
{
	m_Material->SetState(m_State);
	GetMesh()->SetMaterial(m_Material);

	return GetMesh()->Accept(visitor);
}
