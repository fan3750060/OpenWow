#include "stdafx.h"

// General
#include "UISlateNodeFooter.h"

// Additional
#include "Application.h"
#include "UISlateNode.h"


namespace
{
	// Background
	const vec2  cDefaultBackgroundSize  = vec2(240.0f, 32.0f);
	const vec4  cDefaultBackgroundColor = vec4(0.23f, 0.23f, 0.23f, 1.0f);

    // Icon
    const float cDefaultIconSize = 16.0f;
}


CUISlateNodeFooter::CUISlateNodeFooter()
{
    SetSize(cDefaultBackgroundSize);
}

CUISlateNodeFooter::~CUISlateNodeFooter()
{
}



//
// CUISlateNodeFooter
//
void CUISlateNodeFooter::CreateDefault()
{
	m_BackgroungNode = std::make_shared<CUIColorNode>(cDefaultBackgroundSize);
	m_BackgroungNode->SetParentInternal(weak_from_this());
	m_BackgroungNode->SetColor(cDefaultBackgroundColor);


    m_IconNode = std::make_shared<CUITextureNode>(vec2(cDefaultIconSize, cDefaultIconSize));
    m_IconNode->SetParentInternal(weak_from_this());
    m_IconNode->SetTexture(_RenderDevice->CreateTexture2D("Textures\\slate_round.png"));

    // Calculate translate
    vec2 translate = cDefaultBackgroundSize;
    translate.y /= 2.0f;
    translate -= (m_IconNode->GetSize() / 2.0f);
    translate += vec2(1.0f, 1.0f);

    m_IconNode->SetTranslate(translate);
}



//
// CUIBaseNode
//

bool CUISlateNodeFooter::Accept(IVisitor & visitor)
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

	return visitResult;
}

bool CUISlateNodeFooter::AcceptMesh(IVisitor & visitor)
{
	return false;
}
