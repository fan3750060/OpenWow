#include "stdafx.h"

// General
#include "UISlateNodeParameter.h"

// Additional
#include "Application.h"
#include "UISlateNode.h"


namespace
{
	// Background
	const vec2  cDefaultBackgroundSize  = vec2(240.0f, 32.0f);
	const vec4  cDefaultBackgroundColor = vec4(0.43f, 0.43f, 0.43f, 1.0f);

	// Text
	const char* cDefaultText = "Parameter name";
	const uint32 cDefaultTextHeight = 16;
	const vec2  cDefaultTextOffset = vec2(10.0f, 5.0f);
	const vec4  cDefaultTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


CUISlateNodeParameter::CUISlateNodeParameter()
{
    SetSize(cDefaultBackgroundSize);
}

CUISlateNodeParameter::~CUISlateNodeParameter()
{
}



//
// CUISlateNodeParameter
//
void CUISlateNodeParameter::CreateDefault()
{
	m_BackgroungNode = std::make_shared<CUIColorNode>(cDefaultBackgroundSize);
	m_BackgroungNode->SetParentInternal(weak_from_this());
	m_BackgroungNode->SetColor(cDefaultBackgroundColor);

	m_LabelNode = std::make_shared<CUITextNode>();
	m_LabelNode->SetParentInternal(weak_from_this());
	m_LabelNode->SetText(cDefaultText);
	m_LabelNode->SetTranslate(cDefaultTextOffset);
	m_LabelNode->SetColor(cDefaultTextColor);
	m_LabelNode->SetFont(GetManager<IFontsManager>()->Add("Fonts\\JustBreatheBd.otf", cDefaultTextHeight));
}



//
// CUIBaseNode
//

bool CUISlateNodeParameter::Accept(IVisitor & visitor)
{
	bool visitResult = base::Accept(visitor);
	if (!visitResult)
		return false;

	if (m_BackgroungNode != nullptr)
	{
		m_BackgroungNode->Accept(visitor);
	}

	if (m_LabelNode != nullptr)
	{
		m_LabelNode->Accept(visitor);
	}

	return visitResult;
}

bool CUISlateNodeParameter::AcceptMesh(IVisitor & visitor)
{
	return false;
}
