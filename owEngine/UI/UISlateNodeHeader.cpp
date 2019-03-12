#include "stdafx.h"

// General
#include "UISlateNodeHeader.h"

// Additional
#include "Application.h"
#include "UISlateNode.h"


namespace
{
	// Background
	const vec2  cDefaultBackgroundSize  = vec2(240.0f, 56.0f);
	const vec4  cDefaultBackgroundColor = vec4(0.33f, 0.33f, 0.33f, 1.0f);

	// Icon
	const vec2  cDefaultIconOffset = vec2(5.0f, 5.0f);
	const float cDefaultIconSize   = 32.0f;

	// Text
	const char* cDefaultText = "Some test action";
	const uint32 cDefaultTextHeight = 16;
	const vec2  cDefaultTextOffset = vec2(5.0f, 0.0f);
	const vec4  cDefaultTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


CUISlateNodeHeader::CUISlateNodeHeader()
{
    SetSize(cDefaultBackgroundSize);
}

CUISlateNodeHeader::~CUISlateNodeHeader()
{
}



//
// CUISlateNodeHeader
//
void CUISlateNodeHeader::CreateDefault()
{
	m_BackgroungNode = std::make_shared<CUIColorNode>(cDefaultBackgroundSize);
	m_BackgroungNode->SetParentInternal(weak_from_this());
	m_BackgroungNode->SetColor(cDefaultBackgroundColor);

	m_IconNode = std::make_shared<CUITextureNode>(vec2(cDefaultIconSize, cDefaultIconSize));
	m_IconNode->SetParentInternal(weak_from_this());
	m_IconNode->SetTexture(_RenderDevice->CreateTexture2D("Textures\\Operations\\LogMessage.png"));
	m_IconNode->SetTranslate(cDefaultIconOffset);

	vec2 labelPosition = m_IconNode->GetTranslation();
	labelPosition.x += m_IconNode->GetBounds().Width;
	labelPosition += cDefaultTextOffset;

	m_LabelNode = std::make_shared<CUITextNode>();
	m_LabelNode->SetParentInternal(weak_from_this());
	m_LabelNode->SetText(cDefaultText);
	m_LabelNode->SetTranslate(labelPosition);
	m_LabelNode->SetColor(cDefaultTextColor);
	m_LabelNode->SetFont(GetManager<IFontsManager>()->Add("Fonts\\JustBreatheBd.otf", cDefaultTextHeight));
}



//
// CUIBaseNode
//

bool CUISlateNodeHeader::Accept(IVisitor & visitor)
{
	bool visitResult = base::Accept(visitor);
	if (!visitResult)
		return false;

	if (m_BackgroungNode != nullptr)
	{
		m_BackgroungNode->Accept(visitor);
	}

	if (m_IconNode != nullptr)
	{
		m_IconNode->Accept(visitor);
	}

	if (m_LabelNode != nullptr)
	{
		m_LabelNode->Accept(visitor);
	}

	return visitResult;
}

bool CUISlateNodeHeader::AcceptMesh(IVisitor & visitor)
{
	return false;
}
