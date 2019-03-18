#include "stdafx.h"

// General
#include "UISlateNodeParameter_List.h"

// Additional
#include "UISlateEditor.h"
#include "UISlateNode.h"

namespace
{
    // Text
    const char*  cParameterText = "Parameter name";
    const uint32 cParameterTextHeight = 16;
    const vec2   cParameterTextOffset = vec2(10.0f, 5.0f);
    const vec4   cParameterTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUISlateNodeParameter_List::CUISlateNodeParameter_List(std::weak_ptr<CUISlateEditor> Editor)
    : base(Editor)
{
}

CUISlateNodeParameter_List::~CUISlateNodeParameter_List()
{
}



//
// CUISlateNodeParameter_List
//
void CUISlateNodeParameter_List::Initialize(const std::vector<std::string>& List)
{
    base::Initialize();

    m_List = List;

    // Text
    m_Text = std::make_shared<CUITextNode>();
    m_Text->SetParentInternal(weak_from_this());
    m_Text->SetText(cParameterText);
    m_Text->SetTranslate(cParameterTextOffset);
    m_Text->SetTextColor(cParameterTextColor);
    m_Text->SetFont(GetManager<IFontsManager>()->Add("Fonts\\JustBreatheBd.otf", cParameterTextHeight));
    m_Text->Clicked.connect(&CUISlateNodeParameter_List::OnListHeaderClicked, this, std::placeholders::_1);

    glm::vec2 maxListSize = m_Background->GetSize();
    maxListSize.y *= 3.1f;

    m_ListWithActivation = std::make_shared<CUIContainerListWithActivation>();
    m_ListWithActivation->SetParentInternal(weak_from_this());
    m_ListWithActivation->Initialize(maxListSize);
    m_ListWithActivation->SetHeader(m_Text);
    m_ListWithActivation->CreateDefault();
}


//
// CUIBaseNode
//
glm::vec2 CUISlateNodeParameter_List::GetSize() const
{
    // Header already included in param
    if (m_ListWithActivation->IsActive())
        return m_ListWithActivation->GetSize();

    return base::GetSize();
}

std::vector<std::shared_ptr<CUIBaseNode>> CUISlateNodeParameter_List::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    for (auto it : base::GetChilds())
        childs.push_back(it);

    if (m_ListWithActivation)
        childs.push_back(m_ListWithActivation);

    return childs;
}

void CUISlateNodeParameter_List::OnListHeaderClicked(UIBaseNodeClickedEventArgs& e)
{
    m_ListWithActivation->SetActive(! m_ListWithActivation->IsActive());

    std::shared_ptr<CUISlateNode> slateNode = std::dynamic_pointer_cast<CUISlateNode>(GetParent());
    slateNode->CalculateChildsTranslate();
}
